/*
 * object_pool.cpp
 *
 *  Created on: 2020年8月17日
 *      Author: ym
 */

#include "object_pool.hpp"
#include <memory>

using namespace std;

SpeechEvals SpeechEvals::s_instance;

SpeechEvals::SpeechEvals() {
	for (auto i = 0; i < 10; ++i) {
		m_objects.push_back(new_speech());
	}
}

SpeechEvals::~SpeechEvals() {
	for (auto mit = m_objects.begin(); mit != m_objects.end(); ++mit) {
		des_speech(*mit);
	}
}

SpeechEvals *SpeechEvals::instance() {
	return &s_instance;
}

void SpeechEvals::release_speech(SpeechEvaluator *speech) {
	if (speech->Reset() == SPEECH_EVL_OK) {
		lock_guard<mutex> m(m_mutex);
		m_objects.push_back(speech);
	} else {
		des_speech(speech);
	}
}

void SpeechEvals::_release_speech(SpeechEvaluator *speech) {
	s_instance.release_speech(speech);
}

shared_ptr<SpeechEvaluator> SpeechEvals::get_speech() {
	if (m_objects.size() != 0) {
		lock_guard<mutex> m(m_mutex);
		auto mit = m_objects.begin();
		shared_ptr<SpeechEvaluator> sp(*mit, SpeechEvals::_release_speech);
		m_objects.erase(mit);
		return move(sp);
	} else {
		auto speech = new_speech();
		shared_ptr<SpeechEvaluator> sp(speech, SpeechEvals::_release_speech);
		return move(sp);
	}
}

SpeechEvaluator *SpeechEvals::new_speech() {
	int err = 0;
	auto speech = CreateEvalInstance("../det_evl_eng/kh_evl_linux/models/evl.engeneral.child.0.3.1/cfg.json", &err);
	if (err != SPEECH_EVL_OK) {
		std::cout << " init speech error." << std::endl;
		exit(-1);
	} else {
		return speech;
	}
}

void SpeechEvals::des_speech(SpeechEvaluator *speech) {
	FreeEvalInstance(speech);
}
