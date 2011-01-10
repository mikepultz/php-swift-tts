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

	public:
		std::string m_last_error_message;

	public:
		cswift();

		std::string last_error();

		bool init();
		bool shutdown();

		bool set_voice(const char *_voice);
		bool get_voices(std::vector<cswift_voice> &_voices);

		bool generate(const char *_text, uint8_t **_buffer, int *_size);
};

#endif // __CSWIFT_H__
