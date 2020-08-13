#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include "Logger.h"

using namespace std;

jethro::Logger* jethro::Logger::_inst = nullptr;

jethro::Logger::Logger()
    : _fp(NULL), _color(true), _level(Logger::LV_ERROR)
{
}

jethro::Logger::~Logger()
{
    close();
}

bool jethro::Logger::open(const char* filename) {
    if (filename == NULL) {
        _fp = stdout;
        return true;
    }
    _fp = fopen(filename, "a");
    if (_fp == NULL) {
        return false;
    }
    return true;
}

void jethro::Logger::close()
{
    if (_fp != NULL && _fp != stdout) {
        fclose(_fp);
        _fp = NULL;
    }
}

void jethro::Logger::setColor(bool color)
{
    _color = color;
}

void jethro::Logger::setLevel(LEVEL level)
{
    _level = level;
}

void jethro::Logger::log(const LEVEL level, const char* file, const char* func, const int line, const int nl, const char* format, ...)
{
    if (_fp == NULL) {
        return;
    }

    if (level > _level) {
        return;
    }

    std::unique_lock<std::mutex> lock(_mutex);

    switch (level) {
        case Logger::LV_ERROR:
            if (_color) {
                fprintf(_fp, "%s", jethro::color::Red.c_str());
            }
            fprintf(_fp, "[E] ");
            break;
        case Logger::LV_WARNING:
            if (_color) {
                fprintf(_fp, "%s", jethro::color::Yellow.c_str());
            }
            fprintf(_fp, "[W] ");
            break;
        case Logger::LV_DEBUG:
            if (_color) {
                fprintf(_fp, "%s", jethro::color::Reset.c_str());
            }
            fprintf(_fp, "[D] ");
            break;
        default:
            break;
    }

    va_list va;
    va_start(va, format);
    vfprintf(_fp, format, va);
    va_end(va);

    switch (level) {
        case Logger::LV_ERROR:
        case Logger::LV_WARNING:
        case Logger::LV_DEBUG:
            fprintf(_fp, " (%s, %s()::%d)", file, func, line);
            if (_color) {
                fprintf(_fp, "%s", jethro::color::Reset.c_str());
            }
            break;
        default:
            break;
    }
    if (nl == true) {
        fprintf(_fp, "\n");
    }

    fflush(_fp);
    fsync(fileno(_fp));
}

void jethro::Logger::print(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    vfprintf(stdout, format, va);
    va_end(va);

    fflush(stdout);
}

#if 0
int main()
{
    jethro::Logger::getInstance()->open();
//    jethro::Logger::getInstance()->setColor(false);
//    jethro::Logger::getInstance()->setLevel(jethro::Logger::LV_ALL);
//    jethro::Logger::getInstance()->setLevel(jethro::Logger::LV_WARNING);
//    jethro::Logger::getInstance()->setLevel(jethro::Logger::LV_DEBUG);

    info("test1");
    debug("test1");
    warn("test1");
    error("test1");

    prompt("aaaaa");

    return 0;
}
#endif
