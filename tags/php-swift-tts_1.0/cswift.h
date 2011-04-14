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

#ifndef __CSWIFT_H__
#define __CSWIFT_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>

#include "swift.h"

class cswift_voice
{
	public:
		std::string m_id;
		std::string m_name;
		std::string m_path;
		std::string m_version;
		std::string m_buildstamp;
		std::string m_sample_rate;
		std::string m_license_key;
		std::string m_language_tag;
		std::string m_language_name;
		std::string m_language_version;
		std::string m_lexicon_name;
		std::string m_lexicon_version;
		std::string m_speaker_name;
		std::string m_speaker_gender;
		std::string m_speaker_age;

	public:
		cswift_voice();
		cswift_voice(const cswift_voice &_v);
};

class cswift 
{
	private:
		swift_engine *m_engine;
		swift_port *m_port;
		swift_voice *m_voice;
		swift_params *m_params;

		int m_sample_rate;

	public:
		std::string m_last_error_message;

	public:
		cswift();

		std::string last_error();

		int sample_rate();

		bool init();
		bool shutdown();

		bool set_voice(const char *_voice);
		bool get_voices(std::vector<cswift_voice> &_voices);

		bool generate(const char *_text, uint8_t **_buffer, int *_size);
};

#endif // __CSWIFT_H__
