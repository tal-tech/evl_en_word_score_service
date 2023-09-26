/*
 * task.hpp
 *
 *  Created on: 2020年8月18日
 *      Author: ym
 */

#ifndef TASK_HPP_
#define TASK_HPP_

#include <iostream>
#include "../jsoncpp/json/json.h"
#include <sys/stat.h>
#include <vector>
#include "object_pool.hpp"

class Task {
public:
	Task(std::string &url, std::string &id, std::string &body);
	Task(std::string &body);
	~Task();
public:
	bool check_json_body();
	bool check_mime_type();
	// bool check_audio();
	std::string &get_audio();
	bool check_assess_ref();
	bool check_convert_number();
	bool check_vad_max_sec();
	bool check_vad_pause_sec();
	bool check_vad_st_sil_sec();
	bool check_suffix_penal_quick();
	// bool check_need_url();
	bool check_assess_ref_text();
	bool check_assess_ref_core_type();
	bool check_assess_ref_rank();
	bool check_assess_ref_support_repeat();
	bool check_assess_ref_custom_word_pron_object();
	bool check_assess_ref_custom_word_pron_word();
	bool check_assess_ref_custom_word_pron_phone();
	// bool check_upload_oss();
	bool write_audio_file();
	bool parse_alg();
	bool check_decode_audio();
	bool check_audio_size();
	Json::Value &get_result();
	// bool get_need_url();
	Json::Value &get_body();
	// std::string &get_outer_url();
	// std::string &get_inner_url();
private:
	std::string m_body;
	std::string m_url;
	Json::Value m_value;
	std::string m_audio;
	std::string m_file_type;
	std::string m_file_name;
	std::string m_origin_file_name;
	Json::Value m_result;
	std::vector<char> m_wav_audio;
	std::string m_request_id;
	std::string m_outerUrl;
	std::string m_innerUrl;
	struct stat m_st;
};



#endif /* TASK_HPP_ */
