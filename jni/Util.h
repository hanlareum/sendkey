/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdarg.h>

#ifndef SENDKEY_SRC_UTIL_H
#define SENDKEY_SRC_UTIL_H

namespace jethro {

namespace color {
    const std::string Black("\033[0;30m");
    const std::string Red("\033[0;31m");
    const std::string Green("\033[0;32m");
    const std::string Brown("\033[0;33m");
    const std::string Blue("\033[0;34m");
    const std::string Purple("\033[0;35m");
    const std::string Cyan("\033[0;36m");
    const std::string LightGray("\033[0;37m");

    const std::string DarkGray("\033[1;30m");
    const std::string LightRed("\033[1;31m");
    const std::string LightGreen("\033[1;32m");
    const std::string Yellow("\033[1;33m");
    const std::string LightBlue("\033[1;34m");
    const std::string LightPurple("\033[1;35m");
    const std::string LightCyan("\033[1;36m");
    const std::string White("\033[1;37m");

    const std::string Reset("\033[0m");
};

#define SUPPORT_DEBUG_LOG (1)

#ifdef SUPPORT_DEBUG_LOG

#define SUPPORT_DEBUG_LOG_ERROR (1)
//#define SUPPORT_DEBUG_LOG_WARN  (1)
//#define SUPPORT_DEBUG_LOG_DEBUG (1)

inline std::string string_sprintf(const char* fmt, ...){
    int size = 512;
    char* buffer = (char*)calloc(size, sizeof(char));
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if(size<=nsize){
        free(buffer);
        buffer = (char*)calloc(nsize + 1, sizeof(char));
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    free(buffer);
    return ret;
}

#ifdef SUPPORT_DEBUG_LOG_ERROR
#define error(...) do { \
	std::cout << jethro::color::Red << "[E] " << jethro::string_sprintf(__VA_ARGS__) << " (" << __FUNCTION__ << "::" << __LINE__ << ")" << jethro::color::Reset << std::endl; \
} while(0);
#else
#define error(...) do {} while(0);
#endif

#ifdef SUPPORT_DEBUG_LOG_WARN
#define warn(...) do { \
	std::cout << jethro::color::Yellow << "[W] " << jethro::string_sprintf(__VA_ARGS__) << " (" << __FUNCTION__ << "::" << __LINE__ << ")" << jethro::color::Reset << std::endl; \
} while(0);
#else
#define warn(...) do {} while(0);
#endif

#ifdef SUPPORT_DEBUG_LOG_DEBUG
#define debug(...) do { \
	std::cout << jethro::color::Reset << "[D] " << jethro::string_sprintf(__VA_ARGS__) << " (" << __FUNCTION__ << "::" << __LINE__ << ")" << std::endl; \
} while(0);
#else
#define debug(...) do {} while(0);
#endif

#define info(...) do { \
	std::cout << jethro::string_sprintf(__VA_ARGS__) << std::endl; \
} while(0);

#define prompt(...) do { \
    std::cout << jethro::string_sprintf(__VA_ARGS__); \
} while(0);

#else

#define error(...) 	do {} while(0);
#define warn(...) 	do {} while(0);
#define debug(...) 	do {} while(0);
#define info(...) 	do {} while(0);
#define prompt(...) do {} while(0);

#endif /*SUPPORT_DEBUG_LOG*/

void uppercase(std::string &s);

bool startswith(std::string a, std::string b);

std::string strip(std::string& s, const std::string& d = " \t\n\v\r");

int split(std::string s, const char d, std::vector<std::string> &tks);

long str2long(std::string s, int base);

double str2double(std::string s);

void intro(const char* pname);

std::string getTimeNow(std::string format = "%Y%m%d_%H%M%S");

} /*jethro*/

#endif /*SENDKEY_SRC_UTIL_H*/
