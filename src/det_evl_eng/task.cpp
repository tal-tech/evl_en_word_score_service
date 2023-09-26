/*
 * task.cpp
 *
 *  Created on: 2020年8月17日
 *      Author: ym
 */

#include <iostream>
#include "task.hpp"
#include <jsoncpp/json/json.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace boost;
using namespace std;

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Json;

Task::Task(std::string &body)
{
	m_body = body;
	m_file_type = "";
	m_origin_file_name = "";
	m_file_name = "";
	m_request_id = "";
	m_url = "";
	m_innerUrl = "";
	m_outerUrl = "";
}

Task::Task(string &url, string &request_id, string &body) {
	m_body = body;
	m_file_type = "";
	m_origin_file_name = "";
	m_file_name = "";
	m_request_id = request_id;
	m_url = url;
	m_innerUrl = "";
	m_outerUrl = "";
}

Task::~Task() {
	if (m_origin_file_name.size() != 0) {
		unlink(m_origin_file_name.c_str());
	}
}

bool Task::check_json_body() {
	Reader root;
	return root.parse(m_body, m_value);
}

bool Task::check_mime_type() {
	if (!m_value.isMember("mime_type") || !m_value["mime_type"].isString()) {
		std::cout << m_request_id << " mime type not found";
		return false;
	}

	string mime_type = m_value["mime_type"].asString();
	if (mime_type == "audio/mp3") {
		m_file_type = ".mp3";
	} else if (mime_type == "audio/wav") {
		m_file_type = ".wav";
	} else if (mime_type == "audio/ogg") {
		m_file_type = ".ogg";
	} else {
		std::cout << m_request_id << " not support mime type " << mime_type;
		return false;
	}
	return true;
}

// bool Task::check_audio() {
// 	if (!m_value.isMember("audio") || !m_value["audio"].isString() || m_value["audio"].asString().size() == 0) {
// 		std::cout << m_request_id << " audio not found ";
// 		return false;
// 	}

// 	string audio = m_value["audio"].asString();


	// if (!Base64Decode(audio, &m_audio)) {
		//LOG(INFO) << m_request_id << " base64 decode audio error";
		// return false;
	// }

//	if (audio.size() > (1<<20) * 10) {
//		write_audio_file();
//		check_upload_oss();
//		m_value["audio"] = "";
//	}

// 	if (m_audio.size() == 0) {
// 		//LOG(INFO) << m_request_id << " base64 decode audio error";
// 		return false;
// 	}

// 	return true;
// }

std::string &Task::get_audio() {
	return m_audio;
}

bool Task::check_assess_ref() {
	if (!m_value.isMember("assess_ref") || !m_value["assess_ref"].isObject()) {
		std::cout << m_request_id << " assess ref not found.";
		return false;
	}

	return true;
}

bool Task::check_convert_number() {
	if (!m_value.isMember("convert_number")) {
		return true;
	}

	if (!m_value["convert_number"].isBool()) {
		std::cout << m_request_id << " convert number not bool";
		return false;
	}

	m_value["is_convert_number"] = m_value["convert_number"];
	return true;
}

bool Task::check_vad_max_sec() {
	if (!m_value.isMember("vad_max_sec")) {
		return true;
	}

	if (!m_value["vad_max_sec"].isInt()) {
		std::cout << m_request_id << " vad max sec not int";
		return false;
	}

	int vad_max_sec = m_value["vad_max_sec"].asInt();
	if (vad_max_sec < -1 || vad_max_sec > 120) {
		std::cout << m_request_id << " vad max sec inval value : " << vad_max_sec;
		return false;
	}
	return true;
}

bool Task::check_vad_pause_sec() {
	if (!m_value.isMember("vad_pause_sec")) {
		return true;
	}

	if (!m_value["vad_pause_sec"].isInt()) {
		std::cout << m_request_id << " vad pause sec not int";
		return false;
	}

	int vad_pause_sec = m_value["vad_pause_sec"].asInt();
	if (vad_pause_sec < -1 || vad_pause_sec > 90) {
		std::cout << m_request_id << " vad pause sec inval value : " << vad_pause_sec;
		return false;
	}
	return true;
}

bool Task::check_vad_st_sil_sec() {
	if (!m_value.isMember("vad_st_sil_sec")) {
		return true;
	}

	if (!m_value["vad_st_sil_sec"].isInt()) {
		std::cout << m_request_id << " vad st sil sec not int";
		return false;
	}

	int vad_st_sil_sec = m_value["vad_st_sil_sec"].asInt();
	if (vad_st_sil_sec < -1 || vad_st_sil_sec > 90) {
		std::cout << m_request_id << " vad st sil sec inval value " << vad_st_sil_sec;
		return false;
	}
	return true;
}

bool Task::check_suffix_penal_quick() {
	if (!m_value.isMember("suffix_penal_quick")) {
		return true;
	}

	if (!m_value["suffix_penal_quick"].isInt()) {
		std::cout << m_request_id << " suffix penal quick not int";
		return false;
	}

	int vad_st_sil_sec = m_value["suffix_penal_quick"].asInt();
	if (vad_st_sil_sec < -1 || vad_st_sil_sec > 5) {
		std::cout << m_request_id << " suffix penal quick inval value " << vad_st_sil_sec;
		return false;
	}

	m_value["suffix_penal_quick"] = vad_st_sil_sec;
	return true;
}

bool Task::check_assess_ref_text() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("text") || !value["text"].isString()) {
		std::cout << m_request_id << " text not found";
		return false;
	}

	string text = value["text"].asString();
	if (text.size() > 1500 || text.size() == 0) {
		std::cout << m_request_id << " text size > 1500";
		return false;
	}
	return true;
}

bool Task::check_assess_ref_core_type() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("core_type")) {
		return true;
	}

	if (!value["core_type"].isString()) {
		std::cout << m_request_id << " core type not str";
		return false;
	}

	string core_type = value["core_type"].asString();
	if (core_type == "en.word.score") {
		return true;
	} else if (core_type == "en.snt.score") {
		return true;
	} else if (core_type == "en.pred.score") {
		return true;
	} else if (core_type == "en.rec.score") {
		return true;
	} else {
		std::cout << m_request_id << " core type value " << core_type;
		return false;
	}
}

bool Task::check_assess_ref_rank() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("rank")) {
		return true;
	}

	if (!value["rank"].isInt()) {
		std::cout << m_request_id << " rank not int";
		return false;
	}
	int rank = value["rank"].asInt();
	std::cout << m_request_id << " rank vlaue " << rank;
	return (rank >= 3 && rank <= 100);
}

bool Task::check_assess_ref_support_repeat() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("support_repeat")) {
		return true;
	}

	if (!value["support_repeat"].isBool()) {
		std::cout << m_request_id << " support repeat not bool";
		return false;
	}

	return true;
}

bool Task::check_assess_ref_custom_word_pron_object() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("custom_word_pron")) {
		return true;
	}

	if (!value["custom_word_pron"].isArray()) {
		std::cout << m_request_id << " custom word pron not array";
		return false;
	}

	if (value["custom_word_pron"].size() == 0) {
		std::cout << m_request_id << " custom word pron size = 0";
		return false;
	}

	return true;
}

bool Task::check_assess_ref_custom_word_pron_word() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("custom_word_pron")) {
		return true;
	}
	Value prons = value["custom_word_pron"];
	for (unsigned i = 0; i < prons.size(); ++i) {
		if (!prons[i].isMember("word") || !prons[i]["word"].isString()) {
			std::cout << m_request_id << " word not found";
			return false;
		}

		if (prons[i]["word"].asString().size() == 0) {
			std::cout << m_request_id << " word size = 0";
			return false;
		}

		if (prons[i]["word"].asString().size() > 1500) {
			std::cout << m_request_id << " word size > 1500";
			return false;
		}
	}

	return true;
}

bool Task::check_assess_ref_custom_word_pron_phone() {
	Value &value = m_value["assess_ref"];
	if (!value.isMember("custom_word_pron")) {
		return true;
	}
	Value prons = value["custom_word_pron"];
	for (unsigned i = 0; i < prons.size(); ++i) {
		if (!prons[i].isMember("phone") || !prons[i]["phone"].isString()) {
			std::cout << m_request_id << " phone not found";
			return false;
		}

		string str = prons[i]["phone"].asString();
		if (str.size() == 0 || str.size() > 1500) {
			std::cout << m_request_id << " phone size inval";
			return false;
		}
	}
	return true;
}

bool Task::write_audio_file() {
	ofstream  afile;
	uuids::uuid a_uuid = uuids::random_generator()();
	string rand_name = uuids::to_string(a_uuid);

	m_origin_file_name = "/tmp/origin_" + rand_name + m_file_type;
	afile.open(m_origin_file_name);
	if (!afile.is_open()) {
		std::cout << m_request_id << " open " << m_origin_file_name << " error";
		return false;
	}

	// cout << m_audio.size() << endl;
	afile.write(m_audio.c_str(), m_audio.size());
	afile.close();
	return true;
}

bool Task::check_decode_audio() {
	ofstream  afile;
	string cmd;
	uuids::uuid a_uuid = uuids::random_generator()();
	string rand_name = uuids::to_string(a_uuid);

	if (!write_audio_file()) {
		std::cout << m_request_id << " write file error.";
		return false;
	}
	// m_file_name = "/tmp/rand_" + rand_name + ".wav";
	m_file_name = "../唐诗16k.wav";
	cmd = "ffmpeg -loglevel quiet -i " + m_origin_file_name + " -ar 16000 -ac 1 -acodec pcm_s16le " + m_file_name;
	int ret = system(cmd.c_str());
	if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT)) {
		std::cout << m_request_id << " exec " << cmd << " error";
		return false;
	}
	if (access(m_file_name.c_str(), F_OK | R_OK) != 0) {
		std::cout << m_request_id << " decode " << m_file_name << " error";
		return false;
	}

	if (lstat(m_file_name.c_str(), &m_st) != 0) {
		std::cout << m_request_id << " lstat " << m_file_name << " error";
		return false;
	}
	if (m_st.st_size != 0) {
		return true;
	}
	std::cout << m_request_id << " decode error.";
	return false;
}

bool Task::check_audio_size() {
	if (m_st.st_size > (9600102)) {
		std::cout << m_request_id << " wav size to long";
		return false;
	}

	ifstream file;
	file.open(m_file_name);
	if (!file.is_open()) {
		std::cout << m_request_id << " open " << m_file_name << " error.";
		return false;
	}

	m_wav_audio.resize(m_st.st_size);
	file.read(m_wav_audio.data(), m_st.st_size);
	file.close();
	return true;
}

static inline bool parse_oss_result(string &response, string &outer_url, string &inner_url) {
	Reader root;
	Value value;

	if (!root.parse(response, value)) {
		return false;
	}

	if (!value.isMember("code") || !value.isMember("resultBean") || !value["resultBean"].isObject() || \
			!value["resultBean"].isMember("innerUrl") || !value["resultBean"].isMember("outerUrl")) {
		return false;
	}

	outer_url = value["resultBean"]["outerUrl"].asString();
	inner_url = value["resultBean"]["innerUrl"].asString();
	return true;
}

Value &Task::get_result() {
	return m_result;
}


bool Task::parse_alg() {
	Reader root;
	int ret = SPEECH_EVL_OK;
	SpeechEvals *instance = SpeechEvals::instance();
	std::shared_ptr<SpeechEvaluator> speech = instance->get_speech();
	try {
		char const *result = nullptr;
		unsigned int result_len = 0;
		string value = m_value.toStyledString();
		std::cout  << " alg start parse" << endl;
		if ((ret = speech->SetParams(value.c_str(), value.size())) != SPEECH_EVL_OK) {
			std::cout  << " speech set params err value " << ret << endl;
			return false;
		}

		string ref = m_value["assess_ref"].toStyledString();
		if ((ret = speech->Start(ref.c_str(), ref.size())) != SPEECH_EVL_OK) {
			std::cout  << " speech start err value " << ret << endl;
			return false;
		}
		if ((ret = speech->Evaluate(m_wav_audio.data(), m_wav_audio.size(), true, &result, &result_len)) > 2006) {
			std::cout  << " parse evaluate error ret = " << ret << endl;
			return false;
		}

		if (result == nullptr || result_len == 0) {
			std::cout  << " parse det result is nil" << endl;
			return false;
		}

		if (!root.parse(result, result + result_len, m_result)) {
			std::cout  << " parse det result error" << endl;
			return false;
		}
		std::cout  << " alg end parse" << endl;
	} catch (...) {
		std::cout  << " parse det error." << endl;
		return false;
	}
	return true;
}
