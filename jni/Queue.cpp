/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include "Queue.h"

namespace jethro {

Queue::Queue() {
	m_iter = m_queue.begin();
}

void Queue::push(std::string name, std::string code) {
	m_queue.push_back( std::pair<std::string, std::string>(name, code) );
}

void Queue::push(std::pair<std::string, std::string> &key) {
	m_queue.push_back( key );
}

std::pair<std::string, std::string> Queue::pop() {
	if (m_queue.size() > 0) {
		std::pair<std::string, std::string> ret = m_queue.front();
		m_queue.pop_front();
		return ret;
	}
	return std::pair<std::string, std::string>("None", 0);
}

std::list<std::pair<std::string, std::string> >::iterator Queue::first() {
	m_iter = m_queue.begin();
	return m_iter;
}

std::list<std::pair<std::string, std::string> >::iterator Queue::next() {
	return ++m_iter;
}

std::list<std::pair<std::string, std::string> >::iterator Queue::last() {
	return m_queue.end();
}

bool Queue::hasMore() {
	if (m_iter == m_queue.end()) {
		return false;
	}
	return true;
}

int Queue::count() {
	return m_queue.size();
}


} /*jethro*/
