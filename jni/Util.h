/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>
#include <vector>

#include "Logger.h"

#ifndef SENDKEY_SRC_UTIL_H
#define SENDKEY_SRC_UTIL_H

namespace jethro {

void uppercase(std::string &s);

bool startswith(std::string a, std::string b);

std::string strip(std::string& s, const std::string& d = " \t\n\v\r");

int split(std::string s, const char d, std::vector<std::string> &tks);

long str2long(std::string s, int base);

double str2double(std::string s);

void intro(const char* pname);

std::string getTimeNow(std::string format = "%Y%m%d_%H%M%S");

bool execute(const char* cmd);

} /*jethro*/

#endif /*SENDKEY_SRC_UTIL_H*/
