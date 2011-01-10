#include "php_swift.h"
#include "cswift.h"
#include "g711.h"

#ifdef HAVE_SWIFT_LAME
#include "lame/lame.h"
#endif

#ifdef HAVE_SWIFT_GSM
#include "gsm.h"
#define GSM_AUDIO_FRAME 33
#endif

zend_class_entry *sw;
zend_object_handlers swift_object_handlers;

struct swift_object 
{
	zend_object std;
	cswift *sw;
};

void swift_free_handler(void *_object TSRMLS_DC)
{
	swift_object *obj = (swift_object*)_object;
	delete obj->sw;

	zend_hash_destroy(obj->std.properties);
	FREE_HASHTABLE(obj->std.properties);

	efree(obj);

	return;
}
zend_object_value swift_create_handler(zend_class_entry *_type TSRMLS_DC)
{
	zval *tmp;
	zend_object_value retval;

	swift_object *obj = (swift_object*)emalloc(sizeof(swift_object));
	memset(obj, 0, sizeof(swift_object));
	obj->std.ce = _type;

	ALLOC_HASHTABLE(obj->std.properties);
	zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(obj->std.properties, &_type->default_properties,
		(copy_ctor_func_t)zval_add_ref, (void*)&tmp, sizeof(zval*));

	retval.handle = zend_objects_store_put(obj, NULL, swift_free_handler, NULL TSRMLS_CC);
	retval.handlers = &swift_object_handlers;

	return retval;
}

PHP_METHOD(swift, __construct)
{
	swift_object *obj = (swift_object*)zend_object_store_get_object(getThis() TSRMLS_CC);

	obj->sw = new cswift;
	if (obj->sw == NULL)
	{
		// TODO: throw exception
	}

	//
	// init the object
	//
	if (obj->sw->init() == false)
	{
		// TODO: throw exception
	}
}
PHP_METHOD(swift, __destruct)
{
	swift_object *obj = (swift_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	if (obj->sw != NULL)
	{
		if (obj->sw->shutdown() == false)
		{
			// TODO: throw exception
		}
	}
}
PHP_METHOD(swift, setVoice)
{
	char *name = NULL;
	int name_len = 0;

	swift_object *obj = (swift_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	if (obj->sw == NULL)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid swift object.");
		RETURN_FALSE;
	}
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_len) == FAILURE)
	{
		return;
	}
	if (name_len <= 0)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Empty name provided.");
		RETURN_FALSE;
	}

	if (obj->sw->set_voice(name) == false)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, obj->sw->last_error().c_str());
		RETURN_FALSE;
	} else
	{
		RETURN_TRUE;
	}
}
PHP_METHOD(swift, getVoices)
{
	swift_object *obj = (swift_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	if (obj->sw == NULL)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid swift object.");
		RETURN_FALSE;
	}

	std::vector<cswift_voice> voices;

	if (obj->sw->get_voices(voices) == false)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to get list of voices: %s", obj->sw->last_error().c_str());
		RETURN_FALSE;
	} else
	{
		array_init(return_value);

		for(std::vector<cswift_voice>::iterator i=voices.begin(); i!=voices.end(); i++)
		{
			zval *sub_array;

			ALLOC_INIT_ZVAL(sub_array);
			array_init(sub_array);

			add_assoc_string(sub_array, "id",		(char*)i->m_id.c_str(),			1);
			add_assoc_string(sub_array, "path",		(char*)i->m_path.c_str(),		1);
			add_assoc_string(sub_array, "version",		(char*)i->m_version.c_str(),		1);
			add_assoc_string(sub_array, "buildstamp",	(char*)i->m_buildstamp.c_str(),		1);
			add_assoc_string(sub_array, "sample_rate",	(char*)i->m_sample_rate.c_str(), 	1);
			add_assoc_string(sub_array, "license_key",	(char*)i->m_license_key.c_str(), 	1);
			add_assoc_string(sub_array, "language_tag",	(char*)i->m_language_tag.c_str(), 	1);
			add_assoc_string(sub_array, "language_name",	(char*)i->m_language_name.c_str(), 	1);
			add_assoc_string(sub_array, "language_version",	(char*)i->m_language_version.c_str(), 	1);
			add_assoc_string(sub_array, "lexicon_name",	(char*)i->m_lexicon_name.c_str(), 	1);
			add_assoc_string(sub_array, "lexicon_version",	(char*)i->m_lexicon_version.c_str(), 	1);
			add_assoc_string(sub_array, "speaker_name",	(char*)i->m_speaker_name.c_str(), 	1);
			add_assoc_string(sub_array, "speaker_gender",	(char*)i->m_speaker_gender.c_str(), 	1);
			add_assoc_string(sub_array, "speaker_age",	(char*)i->m_speaker_age.c_str(), 	1);
			
			add_assoc_zval(return_value, i->m_name.c_str(), sub_array);
		}
	}
}
PHP_METHOD(swift, generate)
{
	char *text = NULL;
	int text_len = 0;

	long format = -1;
	
	swift_object *obj = (swift_object*)zend_object_store_get_object(getThis() TSRMLS_CC);
	if (obj->sw == NULL)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid swift object.");
		RETURN_FALSE;
	}
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &text, &text_len, &format) == FAILURE)
	{
		return;
	}
	if (text_len <= 0)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Empty text provided.");
		RETURN_FALSE;
	}

	uint8_t *buffer = NULL;
	int size = 0;

	if (obj->sw->generate((const char*)text, &buffer, &size) == false)
	{
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to generate speech: %s", obj->sw->last_error().c_str());
		RETURN_FALSE;
	} else
	{
		char *data = NULL;
		int data_size = 0;

		switch(format)
		{
			//
			// the default format is PCM, which is the return value of the generate function, so we
			// dont' have to do anything- just copy it straight through
			//
			case -1:
			case SWIFT_FORMAT_PCM:
			{
				//
				// copy the buffer into the data array
				//
				data = (char*)emalloc(size);
				if (data != NULL)
				{
					memset(data, 0, size);
					memcpy(data, (char*)buffer, size);

					data_size = size;
				}
			}
			break;

			//
			// WAV format; just a wav container with PCM audio
			//
			case SWIFT_FORMAT_WAV:
			{
				//
				// copy the buffer into the data array
				//
				data = (char*)emalloc(size + 44);
				if (data != NULL)
				{
					memset(data, 0, size);
					char *a = data;

					//
					// create a wav header
					//
					swift_wave_header_t header;
					memset(&header, 0, sizeof(header));

					//
					// RIFF section
					//
					header.m_riff = 0x46464952;
					header.m_file_length = size - 8;
					header.m_wave = 0x45564157;

					//
					// Format section
					//
					header.m_fmt = 0x20746d66;
					header.m_format_length = 16;
					header.m_format = 1;
					header.m_channels = 1;
					header.m_sample_rate = 8000;
					header.m_bytes_per_sec = 16000;
					header.m_block_align = 2;
					header.m_bits_per_sample = 16;

					//
					// data section
					//
					header.m_data = 0x61746164;
					header.m_data_length = size;

					//
					// copy the header and data onto the buffer
					//
					memcpy(a, (char*)&header, sizeof(header));
					a += sizeof(header);
					memcpy(a, (char*)buffer, size);

					data_size = size + 44;
				}
			}
			break;

			//
			// u-law/a-law data; logarithmic representation
			//
			case SWIFT_FORMAT_PCMU:
			{
				//
				// copy the buffer into the data array
				//
				data = (char*)emalloc(size / 2);
				if (data != NULL)
				{
					memset(data, 0, size / 2);
					cg711::ulaw_encode((int16_t*)buffer, (uint8_t*)data, size / 2);
					data_size = size / 2;
				}
			}
			break;
			case SWIFT_FORMAT_PCMA:
			{
				//
				// copy the buffer into the data array
				//
				data = (char*)emalloc(size / 2);
				if (data != NULL)
				{
					memset(data, 0, size / 2);
					cg711::alaw_encode((int16_t*)buffer, (uint8_t*)data, size / 2);
					data_size = size / 2;
				}
			}
			break;

			//
			// GSM- used for telephony- made sense to include support for it; 
			//
#ifdef HAVE_SWIFT_GSM
			case SWIFT_FORMAT_GSM:
			{
				gsm g;
				gsm_frame *frame = NULL;
				int16_t *a = (int16_t*)buffer;
				int esize = 0;

				g = gsm_create();
				if (!g)
				{
					delete[] buffer;
					php_error_docref(NULL TSRMLS_CC, E_ERROR, "failed to initialize GSM encoder.");

					RETURN_FALSE;
				}

				data = (char*)emalloc(size);
				if (data != NULL)
				{
					frame = (gsm_frame*)data;

					for(int i=0; i<(size/2)/160; i++)
					{
						gsm_encode(g, a, *frame);

						frame++;
						a += 160;
						esize += GSM_AUDIO_FRAME;
					}
				}

				gsm_destroy(g);

				data_size = esize;
			}
			break;
#endif
			//
			// MP3 - for everybody else; works well with flash players, but is only supported
			// if the LAME encoder is available on the system
			//
#ifdef HAVE_SWIFT_LAME
			case SWIFT_FORMAT_MP3:
			{
				lame_global_flags *encode_flags = NULL;
				encode_flags = lame_init();
	
				if (encode_flags == NULL)
				{
					delete[] buffer;

					php_error_docref(NULL TSRMLS_CC, E_ERROR, "failed to initialize LAME encoder."); 
					RETURN_FALSE;
				}

				lame_set_num_channels(encode_flags, 1);
				lame_set_in_samplerate(encode_flags, 8000);
				lame_set_brate(encode_flags, 16);
				lame_set_mode(encode_flags, MONO);
				lame_set_quality(encode_flags, 2);

				if (lame_init_params(encode_flags) < 0)
				{
					lame_close(encode_flags);
					delete[] buffer;

					php_error_docref(NULL TSRMLS_CC, E_ERROR, "failed to initialize LAME params."); 
					RETURN_FALSE;
				}

				int buffer_size = (int)ceil(1.25 * (size/2) * sizeof(uint8_t) + 7200);
				char *a = NULL;

				data = (char*)emalloc(buffer_size);
				if (data != NULL)
				{
					memset(data, 0, buffer_size);
					a = data;

					//
					// encode the data
					//
					int esize = lame_encode_buffer(encode_flags, (int16_t*)buffer, (int16_t*)buffer, size / 2, (uint8_t*)a, buffer_size);
					if (esize > 0)
					{
						a += esize;

						//
						// Flush the output buffers
						//
						int esize2 = lame_encode_flush(encode_flags, (uint8_t*)a, buffer_size);
						if (esize2 > 0)
						{
							esize += esize2;
						}

						data_size = esize;
					}
				}

				lame_close(encode_flags);
			}
			break;
#endif
			default:
			{
				delete[] buffer;

				php_error_docref(NULL TSRMLS_CC, E_ERROR, "invalid format flag (%d) provided.", format); 
				RETURN_FALSE;
			}
		}
		
		//
		// delete the buffer returned by generate()
		//
		delete[] buffer;

		//
		// if the data object is NULL, then we failed to convert
		//
		if ( (data == NULL) || (data_size == 0) )
		{
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "failed to encode audio output."); 
			RETURN_FALSE;
		}

		//
		// convert the data to a stream
		//
		php_stream *stream = php_stream_memory_open(TEMP_STREAM_READONLY, data, data_size);
		if (stream == NULL)
		{
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "failed to intialize stream."); 
			RETURN_FALSE;
		}

		php_stream_to_zval(stream, return_value);
	}
}

function_entry swift_methods[] = 
{
	PHP_ME(swift,	__construct,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(swift,	__destruct,	NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(swift,	setVoice,	NULL, ZEND_ACC_PUBLIC)
	PHP_ME(swift,	getVoices,	NULL, ZEND_ACC_PUBLIC)
	PHP_ME(swift,	generate,	NULL, ZEND_ACC_PUBLIC)

	{ NULL, NULL, NULL }
};

PHP_MINIT_FUNCTION(swift_tts)
{
	zend_class_entry s;

	INIT_CLASS_ENTRY(s, PHP_SWIFT_TTS_EXTNAME, swift_methods);
	s.create_object = swift_create_handler;
	sw = zend_register_internal_class(&s TSRMLS_CC);

	memcpy(&swift_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	swift_object_handlers.clone_obj = NULL;

	zend_declare_class_constant_long(sw,	"FORMAT_PCM", 	sizeof("FORMAT_PCM")-1, 	(long)SWIFT_FORMAT_PCM TSRMLS_CC);
	zend_declare_class_constant_long(sw, 	"FORMAT_PCMU", 	sizeof("FORMAT_PCMU")-1, 	(long)SWIFT_FORMAT_PCMU TSRMLS_CC);
	zend_declare_class_constant_long(sw, 	"FORMAT_PCMA", 	sizeof("FORMAT_PCMA")-1, 	(long)SWIFT_FORMAT_PCMA TSRMLS_CC);
	zend_declare_class_constant_long(sw, 	"FORMAT_WAV", 	sizeof("FORMAT_WAV")-1, 	(long)SWIFT_FORMAT_WAV TSRMLS_CC);
#ifdef HAVE_SWIFT_LAME
	zend_declare_class_constant_long(sw, 	"FORMAT_MP3", 	sizeof("FORMAT_MP3")-1, 	(long)SWIFT_FORMAT_MP3 TSRMLS_CC);
#endif
#ifdef HAVE_SWIFT_GSM
	zend_declare_class_constant_long(sw, 	"FORMAT_GSM", 	sizeof("FORMAT_GSM")-1, 	(long)SWIFT_FORMAT_GSM TSRMLS_CC);
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(swift_tts)
{
	std::string formats("PCM, PCMU, PCMA, WAV");

#ifdef HAVE_SWIFT_LAME
	formats += ", MP3";
#endif
#ifdef HAVE_SWIFT_GSM
	formats += ", GSM";
#endif

	php_info_print_table_start();
	php_info_print_table_header(2, "SwiftTTS support", "enabled");
	php_info_print_table_row(2, "SwiftTTS EXT Version", PHP_SWIFT_TTS_EXTVER);
	php_info_print_table_row(2, "Swift library Version", swift_version);
	php_info_print_table_row(2, "Supported Formats", formats.c_str());

#ifdef HAVE_SWIFT_LAME
	php_info_print_table_row(2, "LAME MP3 Encoder Version", get_lame_version());
#else
	php_info_print_table_row(2, "LAME MP3 Encoder Version", "disabled");
#endif
#ifdef HAVE_SWIFT_GSM

	char ver[20];
	(void)sprintf(ver, "%d.%d.%d", GSM_MAJOR, GSM_MINOR, GSM_PATCHLEVEL);

	php_info_print_table_row(2, "GSM Encoder Version", ver);
#else
	php_info_print_table_row(2, "GSM Encoder Version", "disabled");
#endif
	php_info_print_table_end();
}

zend_module_entry swift_tts_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_SWIFT_TTS_EXTNAME,
	NULL,                  /* Functions */
	PHP_MINIT(swift_tts),
	NULL,                  /* MSHUTDOWN */
	NULL,                  /* RINIT */
	NULL,                  /* RSHUTDOWN */
	PHP_MINFO(swift_tts),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SWIFT_TTS_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SWIFT_TTS
extern "C" {
ZEND_GET_MODULE(swift_tts)
}
#endif
