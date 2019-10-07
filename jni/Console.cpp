/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <iostream>
#include <sstream>

#include "Console.h"

using namespace std;

namespace jethro {

Console::Console(std::string header) : header_{header}
{
}

Console::~Console()
{
	turnOnEchoMode();
}

void Console::clearLine(std::stringstream &ss)
{
	ss.seekg(0, ios::end);
	int length = ss.tellg();

	printf("\r");
	for (int i = 0; i < length + 10; i++) {
		printf(" ");
	}
}

void Console::showHistory()
{
	if (history_.size() == 0) {
		std::cout << "empty" << std::endl;
		return;
	}

	std::list<std::string>::iterator iter = history_.begin();
	for (; iter != history_.end(); iter++) {
		std::cout << *iter << std::endl;
	}
}

void Console::turnOnEchoMode()
{
	if (turnedOff_) {
		// system("stty cooked echo");
		attr_.c_iflag |= BRKINT | IGNPAR | ISTRIP | ICRNL | IXON;
		attr_.c_oflag |= OPOST;
		attr_.c_lflag |= ECHO | ISIG | ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &attr_);
		turnedOff_ = false;
	}
}

void Console::turnOffEchoMode()
{
	if (!turnedOff_) {
		// system("stty raw -echo");
		tcgetattr(STDIN_FILENO, &attr_);
		attr_.c_iflag = 0;
		attr_.c_oflag &= ~OPOST;
		attr_.c_lflag &= ~(ECHO | ISIG | ICANON);
		attr_.c_cc[VMIN] = 1;
		attr_.c_cc[VTIME] = 0;
		tcsetattr (STDIN_FILENO, TCSANOW, &attr_);
		turnedOff_ = true;
	}
}

#if 0
int getIndexByIter(std::list<std::string> &history, std::list<std::string>::iterator &search) {
	int index = -1;
	std::list<std::string>::iterator iter = history.begin();
	for (; iter != history.end(); iter++) {
		if (search == iter) {
			index++;
			break;
		}
		index++;
	}
	return index;
}
#endif

std::string Console::readLine()
{
	char ch[3];
	int idx = 0;

	bool cached = false;
	bool historiable = false;

	std::string cache = "";
	std::stringstream buffer;

	std::list<std::string>::iterator iter = history_.end();
	iter--;

	turnOffEchoMode();
	do {
		ch[idx] = getchar();
		if (isalnum(ch[idx]) && idx == 0) {
			if (cached) {
				buffer.str("");
				buffer.clear();
				cache = "";
				cached = false;
			}
			buffer << ch[idx];
			historiable = true;
			printf("\r%s%s", header_.c_str(), buffer.str().c_str());
		} else {
			if (ch[idx] == 127 || ch[idx] == 8) { /*BACKSPACE*/
				std::string tmp = buffer.str();
				if (tmp != "") {
					clearLine(buffer);

					buffer.str("");
					buffer.clear();

					tmp = tmp.substr(0, tmp.length()-1);

					buffer << tmp;
					printf("\r%s%s", header_.c_str(), buffer.str().c_str());
					historiable = true;
				}
			} else if (ch[idx] == 3) { /*cTRL + C*/
				turnOnEchoMode();
				std::cout << std::endl;
				exit(0);
			} else if (ch[0] == 27 && ch[1] == 91) {
				switch(ch[2]) {
				case 65:
					clearLine(buffer);
					if (!cached) {
						cache = buffer.str();
						cached = true;
					}
					if (history_.size() > 0) {
						buffer.str("");
						buffer.clear();

						if (iter == history_.end()) {
							iter--;
						}
						buffer << (*iter).c_str();
						if (iter != history_.begin()) {
							iter--;
						}
						//printf("up (%d)", idx);
					}
					printf("\r%s%s", header_.c_str(), buffer.str().c_str());
					historiable = true;
					break;
				case 66:
					clearLine(buffer);
					if (!cached) {
						cache = buffer.str();
						cached = true;
					}
					if (history_.size() > 0) {
						if (iter != history_.end()) {
							buffer.str("");
							buffer.clear();

							if (iter == history_.begin()) {
								iter++;
							}
							buffer << (*iter).c_str();
							iter++;
						}
						//printf("down");
					}
					printf("\r%s%s", header_.c_str(), buffer.str().c_str());
					historiable = true;
					break;
				}
			} else {
				 //printf("\r%d [%d]", ch[idx], idx);
			}
			idx++;
			if (idx == 3) {
				idx = 0;
				ch[0] = ch[1] = ch[2] = 0;
			}
		}
	} while(ch[idx > 0 ? idx - 1 : 0] != 13);

	turnOnEchoMode();
	std::cout << std::endl;
	if (historiable) {
		std::string data = buffer.str();
		if (data == "history") {
			showHistory();
			history_.push_back(data);
			data = "";
		} else {
			history_.push_back(data);
		}
		return data;
	}
	return "";
}

} /*jethro*/
