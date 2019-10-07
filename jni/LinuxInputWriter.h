/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <list>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "Queue.h"
#include "LinuxInput.h"

#ifndef SENDKEY_SRC_LINUX_INPUT_WRITER_H
#define SENDKEY_SRC_LINUX_INPUT_WRITER_H

namespace jethro {

class LinuxInputWriter : public jethro::Queue {
public:
	static std::shared_ptr<LinuxInputWriter> create(std::shared_ptr<jethro::LinuxInput> device);

	virtual ~LinuxInputWriter();

	void exec(std::string name);
	void exec(std::pair<std::string, std::string> key);

private:
	LinuxInputWriter(std::shared_ptr<jethro::LinuxInput> device);

	bool initialize();

	void mainloop();

	bool m_running;

	std::mutex m_mutex;
	std::thread m_thread;
	std::condition_variable m_condvar;
	std::shared_ptr<jethro::LinuxInput> m_device;
}; /*LinuxInputWriter*/

} /*jethro*/

#endif /*SENDKEY_SRC_LINUX_INPUT_WRITER_H*/
