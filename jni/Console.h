/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <list>
#include <string>

#include <termios.h>

#ifndef SENDKEY_SRC_CONSOLE_H
#define SENDKEY_SRC_CONSOLE_H

namespace jethro {

class Console {
public:
	Console(std::string header = "");
	virtual ~Console();

	std::string readLine();

private:
	std::string header_;
	std::list<std::string> history_;

	struct termios attr_;
	bool turnedOff_ = false;

	void clearLine(std::stringstream &ss);
	void showHistory();

	void turnOnEchoMode();
	void turnOffEchoMode();
}; /*Console*/

} /*jethro*/

#endif /*SENDKEY_SRC_CONSOLE_H*/
