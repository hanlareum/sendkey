/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>
#include <memory>

#include "Util.h"
#include "Database.h"


#define VT_CONSOLE_SUPPORT (1)

#ifndef SENDKEY_SRC_LINUX_INPUT_H
#define SENDKEY_SRC_LINUX_INPUT_H

namespace jethro {

class LinuxInput {
public:
	static std::shared_ptr<LinuxInput> create(std::string name, std::string path, std::shared_ptr<Database> db);

	virtual ~LinuxInput();
	
	void emitKey(int key);
	void emitKey(std::string &name);

private:
	LinuxInput(std::string name, std::string path, std::shared_ptr<Database> db);

	bool destroy();

	void emit(int type, int code, int value, struct timeval *tv = 0);

	bool initialize();

	std::string m_name;
	std::string m_path;

	int m_fd;
#ifdef VT_CONSOLE_SUPPORT
	int m_tty_fd;
#endif
	std::shared_ptr<Database> m_db;
}; /*LinuxInput*/

} /*jethro*/

#endif /*SENDKEY_SRC_LINUX_INPUT_H*/
