/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>

#include "LinuxEvent.h"

namespace jethro {

std::shared_ptr<LinuxEvent> LinuxEvent::create(std::string path, std::shared_ptr<Database> db) {
	if (path.empty()) {
		error("device path is empty");
		return nullptr;
	}
	if (db == nullptr || db == 0) {
		error("database is null");
		return nullptr;
	}
	std::shared_ptr<LinuxEvent> event(new LinuxEvent(path, db));
	if (event && event->initialize()) {
		return event;
	}
	error("fail to create linux event");
	return nullptr;
}

LinuxEvent::~LinuxEvent() {
	if (m_fd >= 0) {
		close(m_fd);
		m_fd = -1;
	}
}

LinuxEvent::LinuxEvent(std::string path, std::shared_ptr<Database> db) 
	: m_name{""}, 
	  m_path{path}, 
	  m_fd{-1}, 
	  m_db{db} {
}

bool LinuxEvent::initialize() {
	m_fd = ::open(m_path.c_str(), O_RDONLY);
	if (m_fd < 0) {
		error("fail to open device -> %s, %s", m_name.c_str(), m_path.c_str());
		return false;
	}

	char name[256] = {0,};
	if (::ioctl(m_fd, EVIOCGNAME(sizeof(name) - 1), name) < 1) {
		m_name = name;
	}

	m_pollfds.push_back(pollfd{m_fd, POLLIN, 0});

	return true;
}

int LinuxEvent::read(long &code, long &value)
{
	if (m_pollfds.empty()) {
		error("no pollfds!!");
		return 0;
	}

	// vector<vector<input_event>> events(poll_fds.size());
	::poll(m_pollfds.data(), m_pollfds.size(), 1000);
	for (size_t i = 0; i < m_pollfds.size(); i++) {
		if (m_pollfds[i].revents & POLLIN) {
			struct input_event event;
			int rc = ::read(m_pollfds[i].fd, &event, sizeof(event));
			if (rc == sizeof(event)) {
				if (event.type == EV_KEY) {
					code = event.code;
					value = event.value;
				}
				return 1;
			}
			error("failed to read an event. (%d != %d)", rc, sizeof(event));
		}
	}
	return 0;
}

std::string LinuxEvent::getNameByCode(long code)
{
	std::string name = m_db->findByCode(code);
	if (name.empty()) {
		name = std::string("UNKNOWN(") + std::to_string(code) + std::string(")");
	}
	return name;
}

} /*jethro*/