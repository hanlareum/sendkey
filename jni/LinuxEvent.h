/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>
#include <memory>
#include <vector>
#include <sys/poll.h>

#include "Util.h"
#include "Database.h"

#ifndef SENDKEY_SRC_LINUX_EVENT_H
#define SENDKEY_SRC_LINUX_EVENT_H

namespace jethro {

class LinuxEvent {
public:
	static std::shared_ptr<LinuxEvent> create(std::string path, std::shared_ptr<Database> db);

	virtual ~LinuxEvent();
	
	int read(long &code, long &value);
	std::string getNameByCode(long code);

private:
	LinuxEvent(std::string path, std::shared_ptr<Database> db);

	bool initialize();

	std::string m_name;
	std::string m_path;

	int m_fd;

	std::shared_ptr<Database> m_db;

	std::vector<pollfd> m_pollfds;
}; /*LinuxEvent*/

} /*jethro*/

#endif /*SENDKEY_SRC_LINUX_EVENT_H*/
