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
#include "zend_exceptions.h"

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
