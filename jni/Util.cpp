/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <iostream>
#include <sstream>
#include <algorithm>

#include <string.h>
#include <libgen.h>

#include "Util.h"
#include "version.h"

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

void intro(std::string pname) {
	prompt("%s%s %s (v%d, %s)%s\n", jethro::color::White.c_str(), basename(pname.c_str()), VERSION, AUTHOR, EMAIL, jethro::color::Reset.c_str());
}

} /*jethro*/