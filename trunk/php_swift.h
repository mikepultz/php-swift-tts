#ifndef PHP_SWIFT_TTS_H
#define PHP_SWIFT_TTS_H

#define PHP_SWIFT_TTS_EXTNAME  "SwiftTTS"
#define PHP_SWIFT_TTS_EXTVER   "1.0"

#include <stdint.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif 

extern "C" {
#include "php.h"
}

#include "ext/standard/info.h"

extern zend_module_entry swift_tts_module_entry;
#define phpext_swift_tts_ptr &swift_tts_module_entry;

enum audio_format_t
{
	SWIFT_FORMAT_PCM,
	SWIFT_FORMAT_PCMU,
	SWIFT_FORMAT_PCMA,
#ifdef HAVE_SWIFT_LAME
	SWIFT_FORMAT_MP3,
#endif
#ifdef HAVE_SWIFT_GSM
	SWIFT_FORMAT_GSM,
#endif
	SWIFT_FORMAT_WAV
};

typedef struct
{
	//
	// RIFF header
	//
	uint32_t m_riff;
	uint32_t m_file_length;
	uint32_t m_wave;
	
	//
	// format header
	//
	uint32_t m_fmt;
	uint32_t m_format_length;
	uint16_t m_format;
	uint16_t m_channels;
	uint32_t m_sample_rate;
	uint32_t m_bytes_per_sec;
	uint16_t m_block_align;
	uint16_t m_bits_per_sample;

	//
	// Data header
	//
	uint32_t m_data;
	uint32_t m_data_length;

} swift_wave_header_t;

#endif /* PHP_SWIFT_TTS_H */
