//============================================================================
// Name        : engeneral.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <thread>
#include <future>
#include <unistd.h>
#include "object_pool.hpp"
#include "task.hpp"
using namespace std;
using namespace Json;

Value get_value() {
	Value v;
	v["audio"] = "text";
	v["mime_type"] = "audio/wav";
	Value ref;
	ref["text"] = "who am i";
	ref["core_type"] = "en.word.score"; //英文单词语音评测
	v["assess_ref"] = ref;
	return v;
}

int main() {
	Value v = get_value();
	string body = v.toStyledString();
	Task task(body);

	if (!task.check_json_body()) {
		cout << "parse json error" << endl;
		return 0;
	}

	if (!task.check_mime_type()) {
		cout << "check mime type error " << endl;
		return 0;
	}

	if (!task.check_assess_ref()) {
		cout << "check assess ref error" << endl;
		return 0;
	}

	if (!task.check_assess_ref_text()) {
		cout << "check assess ref text error." << endl;
		return 0;
	}

	if (!task.check_decode_audio()) {
		cout << "check assess audio error" << endl;
		return 0;
	}

	if (!task.check_audio_size()) {
		cout << "check audio size erorr" << endl;
		return 0;
	}

	if (!task.parse_alg()) {
		cout << "check parse alg error." << endl;
		return 0;
	}

	cout << task.get_result() << endl;
	return 0;
}
