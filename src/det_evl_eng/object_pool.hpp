/*
 * object_pool.hpp
 *
 *  Created on: 2020年8月17日
 *      Author: ym
 */

#ifndef OBJECT_POOL_HPP_
#define OBJECT_POOL_HPP_

#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include "kh_evl_linux/speech_evaluator.h"

#define SPEECH_EVL_OK 1000

class SpeechEvals {
public:
	static SpeechEvals *instance();
	std::shared_ptr<SpeechEvaluator> get_speech();
private:
	static void _release_speech(SpeechEvaluator *);
	void release_speech(SpeechEvaluator *);
	SpeechEvaluator *new_speech();
	void des_speech(SpeechEvaluator *speech);
private:
	SpeechEvals();
	~SpeechEvals();
	SpeechEvals(const SpeechEvals &sps);
	SpeechEvals& operator=(const SpeechEvals &);
private:
	std::mutex m_mutex;
	std::list<SpeechEvaluator *> m_objects;
private:
	static SpeechEvals s_instance;
};

#endif /* OBJECT_POOL_HPP_ */
