/**
 * Swift Text-To-Speech Extension for PHP5      
 *
 * Copyright (c) 2011, Mike Pultz <mike@mikepultz.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *   * Neither the name of Mike Pultz nor the names of his contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRIC
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author      Mike Pultz <mike@mikepultz.com>
 * @copyright   2011 Mike Pultz <mike@mikepultz.com>
 * @license     http://www.opensource.org/licenses/bsd-license.php  BSD License
 * @version     SVN $id$
 *
 */

#include "cswift.h"

cswift_voice::cswift_voice() : m_id(), m_name(), m_path(), m_version(), m_buildstamp(), m_sample_rate(), m_license_key(), 
	m_language_tag(), m_language_name(), m_language_version(), m_lexicon_name(), m_lexicon_version(), 
	m_speaker_name(), m_speaker_gender(), m_speaker_age()
{
}
cswift_voice::cswift_voice(const cswift_voice &_v)
{
	this->m_id			= _v.m_id;
	this->m_name 			= _v.m_name;
	this->m_path			= _v.m_path;
	this->m_version			= _v.m_version;
	this->m_buildstamp		= _v.m_buildstamp;
	this->m_sample_rate		= _v.m_sample_rate;
	this->m_license_key		= _v.m_license_key;
	this->m_language_tag		= _v.m_language_tag;
	this->m_language_name		= _v.m_language_name;
	this->m_language_version	= _v.m_language_version;
	this->m_lexicon_name		= _v.m_lexicon_name;
	this->m_lexicon_version		= _v.m_lexicon_version;
	this->m_speaker_name 		= _v.m_speaker_name;
	this->m_speaker_gender 		= _v.m_speaker_gender;
	this->m_speaker_age 		= _v.m_speaker_age;
}

cswift::cswift() : m_last_error_message()
{
	this->m_engine = NULL;
	this->m_port = NULL;
	this->m_voice = NULL;
	this->m_params = NULL;

	//
	// this is set when you select a voice
	//
	this->m_sample_rate = 8000;
}
std::string cswift::last_error()
{
	return this->m_last_error_message;
}
int cswift::sample_rate()
{
	return this->m_sample_rate;
}
bool cswift::init()
{
	//
	// open the engine
	//
	this->m_engine = swift_engine_open(NULL);
	if (this->m_engine == NULL)
	{
		this->m_last_error_message = "failed to initialize swift engine.";
		return false;
	}

	//
	// setup the params
	//
	this->m_params = swift_params_new(NULL);
	if (this->m_params == NULL)
	{
		this->m_last_error_message = "failed to initialize swift params.";
		return false;
	}

	swift_params_set_string(this->m_params, "audio/encoding", "pcm16");
	swift_params_set_int(this->m_params, "audio/channels", 1);
	swift_params_set_int(this->m_params, "audio/deadair", 0);

	//
	// open the swift port
	//
	this->m_port = swift_port_open(this->m_engine, this->m_params);
	if (this->m_port == NULL)
	{
		this->m_last_error_message = "failed to initialize swift port.";
		return false;
	}

	return true;
}
bool cswift::shutdown()
{
	if (this->m_port != NULL)
	{
		swift_port_close(this->m_port);
	}
	if (this->m_engine != NULL)
	{
		swift_engine_close(this->m_engine);
	}

	return true;
}
bool cswift::set_voice(const char *_voice)
{
	char *s = NULL;

	if (this->m_port == NULL)
	{
		this->m_last_error_message = "invalid swift port; bad object.";
		return false;
	}
	if (_voice == NULL)
	{
		this->m_last_error_message = "empty voice provided.";
		return false;
	}

	//
	// set the voice
	//
	this->m_voice = swift_port_set_voice_by_name(this->m_port, _voice);
	if (this->m_voice == NULL)
	{
		this->m_last_error_message = "failed to set swift voice; unknown voice: ";
		this->m_last_error_message += _voice;

		return false;
	}
	if (SWIFT_FAILED(swift_port_set_voice(this->m_port, this->m_voice)))
	{
		this->m_last_error_message = "failed to set swift voice; unknown voice: ";
		this->m_last_error_message += _voice;

		return false;
	}

	//
	// store the sample rate for encoding later
	//
	s = (char*)swift_voice_get_attribute(this->m_voice, "sample-rate");
	if (s != NULL)
	{
		this->m_sample_rate = atoi(s);
	}

	return true;		
}
bool cswift::get_voices(std::vector<cswift_voice> &_voices)
{
	if (this->m_port == NULL)
	{
		this->m_last_error_message = "invalid swift port; bad object.";
		return false;
	}

	//
	// search for voices
	//
	swift_voice *voice = swift_port_find_first_voice(this->m_port, NULL, NULL);
	if (voice != NULL)
	{
		while(voice != NULL)
		{
			cswift_voice v;
			char *s = NULL;

			s = (char*)swift_voice_get_attribute(voice, "id");
			if (s != NULL)
			{
				v.m_id = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "name");
			if (s != NULL)
			{
				v.m_name = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "path");
			if (s != NULL)
			{
				v.m_path = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "version");
			if (s != NULL)
			{
				v.m_version = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "buildstamp");
			if (s != NULL)
			{
				v.m_buildstamp = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "sample-rate");
			if (s != NULL)
			{
				v.m_sample_rate = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "license/key");
			if (s != NULL)
			{
				v.m_license_key = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "language/tag");
			if (s != NULL)
			{
				v.m_language_tag = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "language/name");
			if (s != NULL)
			{
				v.m_language_name = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "language/version");
			if (s != NULL)
			{
				v.m_language_version = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "lexicon/name");
			if (s != NULL)
			{
				v.m_lexicon_name = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "lexicon/version");
			if (s != NULL)
			{
				v.m_lexicon_version = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "speaker/name");
			if (s != NULL)
			{
				v.m_speaker_name = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "speaker/gender");
			if (s != NULL)
			{
				v.m_speaker_gender = s;
			}

			s = (char*)swift_voice_get_attribute(voice, "speaker/age");
			if (s != NULL)
			{
				v.m_speaker_age = s;
			}

			_voices.push_back(v);

			voice = swift_port_find_next_voice(this->m_port);
		}
	} else
	{
		this->m_last_error_message = "no voices found on this system.";
		return false;
	}

	return true;
}
bool cswift::generate(const char *_text, uint8_t **_buffer, int *_size)
{
	if (this->m_port == NULL)
	{
		this->m_last_error_message = "invalid swift port; bad object.";
		return false;
	}
	if (this->m_voice == NULL)
	{
		this->m_last_error_message = "you must select a voice to use before you generate.";
		return false;
	}
	if (_text == NULL)
	{
		this->m_last_error_message = "you must provide some text to generate.";
		return false;
	}

	swift_waveform *wave = NULL;
	int16_t *buffer = NULL;
	int samples = 0;
	int bytes_per_sample = 0;

	//
	// allocate the waveform to hold the response
	//
	wave = swift_waveform_new();
	if (wave == NULL)
	{
		this->m_last_error_message = "failed to allocate waveform object.";
		return false;
	}

	//
	// speak the text, and put it directly into a waveform
	//
	wave = swift_port_get_wave(this->m_port, _text, strlen(_text), NULL, false, NULL);

	//
	// get the samples from the waveform object
	//
	swift_result_t rv = swift_waveform_get_samples(wave, (const void**)&buffer, &samples, &bytes_per_sample);
	if (SWIFT_FAILED(rv))
	{
		this->m_last_error_message = "failed to copy samples from waveform.";
		return false;
	}

	//
	// copy the samples out of the object, and push them into our output buffer
	//
	if ( (samples > 0) && (bytes_per_sample > 0) )
	{
		*_size = samples * bytes_per_sample;

		*_buffer = new uint8_t[*_size];
		if (*_buffer == NULL)
		{
			this->m_last_error_message = "failed to malloc buffer";
			return false;
		}

		memset(*_buffer, 0, *_size);
		memcpy(*_buffer, buffer, *_size);

	} else
	{
		char message[100];
		(void)sprintf(message, "invalid waveform: samples=%d, bytes_per_sample=%d", samples, bytes_per_sample);
		this->m_last_error_message = message;

                return false;
        }

	//
	// close and de-allocate teh waveform
	//
	swift_waveform_close(wave);

	return true;
}
