/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <iostream>

#include "LinuxInputWriter.h"

namespace jethro {

std::shared_ptr<LinuxInputWriter> LinuxInputWriter::create(std::shared_ptr<jethro::LinuxInput> device) {
	if (device == nullptr) {
		error("device is not set!!");
		return nullptr;
	}
	std::shared_ptr<LinuxInputWriter> thread(new LinuxInputWriter(device));
	if (thread && thread->initialize()) {
		return thread;
	}
	error("fail to create exec thread");
	return nullptr;
}

LinuxInputWriter::LinuxInputWriter(std::shared_ptr<jethro::LinuxInput> device) : m_running{false}, m_device{device} {
}

LinuxInputWriter::~LinuxInputWriter() {
	m_running = false;
	m_condvar.notify_one();
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

bool LinuxInputWriter::initialize() {
	m_running = true;
	m_thread = std::thread(&LinuxInputWriter::mainloop, this);
	return true;	
}

void LinuxInputWriter::mainloop() {
	std::string keyName, keyCode;

	while (true) {
		std::unique_lock<std::mutex> lock(m_mutex);
		if (count() == 0 && m_running) {
			m_condvar.wait(lock, [this]() { return (count() > 0 || !m_running); });
		}

		if (!m_running) {
			return;
		}

		std::tie(keyName, keyCode) = pop();
		lock.unlock();

		if (keyCode == "") {
			m_device->emitKey(keyName);
		} else {
			m_device->emitKey(keyName, keyCode);
		}
	}

	m_running = false;
}

void LinuxInputWriter::exec(std::string name) {
	exec(std::pair<std::string, std::string>(name, ""));
}

void LinuxInputWriter::exec(std::string name, std::string code) {
	exec(std::pair<std::string, std::string>(name, code));
}

void LinuxInputWriter::exec(std::pair<std::string, std::string> key) {
	push(key);
	m_condvar.notify_one();
}

} /*jethro*/
