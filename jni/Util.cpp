/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <iostream>
#include <sstream>
#include <algorithm>

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <iomanip> // put_time
#include <string>  // string

#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "Util.h"
#include "Version.h"

using namespace std;

namespace jethro {

std::string strip(std::string& s, const std::string& d) {
	std::string r = s.erase(s.find_last_not_of(d) + 1);
	return r.erase(0, r.find_first_not_of(d));
}

int split(std::string s, const char d, std::vector<std::string> &tks) {
	s = jethro::strip(s);
	if (!s.empty()) {
		std::string tk;
		std::stringstream ss(s);
		while (std::getline(ss, tk, d)) {
			tk = jethro::strip(tk);
			if (!tk.empty()) {
				tks.push_back(tk);
			}
		}
		return tks.size();
	}
	return 0;
}

long str2long(std::string s, int base) {
	char *p = 0;
	return strtol(s.c_str(), &p, base);
}

double str2double(std::string s) {
	std::string::size_type sz;
	return std::stod (s, &sz);
}

void uppercase(std::string &s) {
	s = jethro::strip(s);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) -> unsigned char {return std::toupper(c);});
}

bool startswith(std::string a, std::string b) {
	return (strncmp(a.c_str(), b.c_str(), b.length()) == 0) ? true : false;
}

void intro(const char* pname) {
	prompt("%s%s v%.1f (%s, %s)%s\n", jethro::color::White.c_str(), basename(pname), VERSION, AUTHOR, EMAIL, jethro::color::Reset.c_str());
	usleep(100 * 1000);
}

std::string getTimeNow(std::string format) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), format.c_str());
    return ss.str();
}

} /*jethro*/
