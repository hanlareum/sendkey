/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <linux/kd.h>

#include "LinuxInput.h"

#define ioctl1(a) { \
		if (ioctl(m_fd, a) < 0) { \
			error("ioctl(%s) fail", #a); \
			return false; \
		} \
	};
#define ioctl2(a,b) { \
		if (ioctl(m_fd, a, b) < 0) { \
			error("ioctl(%s,%s) fail", #a, #b); \
			return false; \
		} \
	};

namespace jethro {

std::shared_ptr<LinuxInput> LinuxInput::create(
		std::string name, std::string path, 
		std::shared_ptr<Database> db) {
	if (name.empty()) {
		error("device name is empty");
		return nullptr;
	}
	if (path.empty()) {
		error("device path is empty");
		return nullptr;
	}
	if (db == nullptr || db == 0) {
		error("database is null");
		return nullptr;
	}
	std::shared_ptr<LinuxInput> input(new LinuxInput(name, path, db));
	if (input && input->initialize()) {
		return input;
	}
	error("fail to create linux input");
	return nullptr;
}

LinuxInput::~LinuxInput() {
	if (m_fd >= 0) {
		destroy();
		close(m_fd);
	}
#ifdef VT_CONSOLE_SUPPORT
	if (m_tty_fd >= 0) {
		close(m_tty_fd);
	}
#endif
}

void LinuxInput::emitKey(int key) {
	emit(EV_KEY, key, 1);
	emit(EV_SYN, SYN_REPORT, 0);
	emit(EV_KEY, key, 0);
	emit(EV_SYN, SYN_REPORT, 0);
}

void LinuxInput::emitKey(std::string &name) {
	long code = m_db->findByName(name);
	if (code >= 0) {
		emitKey(code);
	}
}

LinuxInput::LinuxInput(
		std::string name, 
		std::string path, 
		std::shared_ptr<Database> db) 
	: m_name{name}, 
	  m_path{path}, 
	  m_fd{-1},
#ifdef VT_CONSOLE_SUPPORT
	  m_tty_fd{-1}, 
#endif
	  m_db{db} {
}

bool LinuxInput::destroy() {
	ioctl1(UI_DEV_DESTROY);
	return true;
}

void LinuxInput::emit(int type, int code, int value, struct timeval *tv) {
	struct input_event ie;

	ie.type = type;
	ie.code = code;
	ie.value = value;

	if (tv == 0) {
		ie.time.tv_sec = 0;
		ie.time.tv_usec = 0;
	} else {
		ie.time.tv_sec = tv->tv_sec;
		ie.time.tv_usec = tv->tv_usec;
	}
	::write(m_fd, &ie, sizeof(ie));

#ifdef VT_CONSOLE_SUPPORT
	{
		struct kbentry ke;
		ke.kb_table = K_NORMTAB; /* K_SHIFTTAB or K_NORMTAB */
		ke.kb_index = code;
		ke.kb_value = 0;
		if (::ioctl(m_tty_fd, KDGKBENT, &ke)) {
			return;
		}
		if (ke.kb_value) {
			/* do something special for ASCII key handling... */
		}
	}
#endif
}

bool LinuxInput::initialize() {
	struct uinput_user_dev uidev;

	m_fd = ::open(m_path.c_str(), O_WRONLY|O_NONBLOCK);
	if (m_fd < 0) {
		error("fail to open device -> %s, %s", m_name.c_str(), m_path.c_str());
		return false;
	}

	auto iter = m_db->begin();
	ioctl2(UI_SET_EVBIT, EV_KEY);
	for (; iter != m_db->end(); ++iter) {
		ioctl2(UI_SET_KEYBIT, iter->second);
	}

	ioctl2(UI_SET_EVBIT, EV_ABS);
	ioctl2(UI_SET_ABSBIT, ABS_X);
	ioctl2(UI_SET_ABSBIT, ABS_Y);
	ioctl2(UI_SET_ABSBIT, ABS_Z);
	ioctl2(UI_SET_ABSBIT, ABS_PRESSURE);
	ioctl2(UI_SET_KEYBIT, BTN_TOUCH);

	ioctl2(UI_SET_EVBIT, EV_REL);
	ioctl2(UI_SET_KEYBIT, BTN_MOUSE);
	ioctl2(UI_SET_KEYBIT, BTN_LEFT);
	ioctl2(UI_SET_KEYBIT, BTN_RIGHT);
	ioctl2(UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl2(UI_SET_KEYBIT, BTN_FORWARD);
	ioctl2(UI_SET_KEYBIT, BTN_BACK);

	memset(&uidev, 0, sizeof(uidev));

	uidev.id.bustype = BUS_USB;
	uidev.id.version = 1;
	uidev.id.vendor  = 0xFF;
	uidev.id.product = 0xFF;
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "%s", m_name.c_str());

	if (write(m_fd, &uidev, sizeof(uidev)) < 0) {
		error("fail to write(uinput_user_dev)");
		return false;
	}
	ioctl1(UI_DEV_CREATE);

#ifdef VT_CONSOLE_SUPPORT
	m_tty_fd = ::open("/dev/tty0", O_RDWR);
#endif

	return true;
}

} /*jethro*/
