/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <mutex>
#include <string>

#ifndef SENDKEY_SRC_LOGGER_H
#define SENDKEY_SRC_LOGGER_H

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

class Logger {
public:
    typedef enum {
        LV_NONE = 0
        , LV_INFO
        , LV_ERROR
        , LV_WARNING
        , LV_DEBUG
        , LV_ALL
    } LEVEL;

public:
    bool open(const char* filename);
    void close();

    void setColor(bool color);
    void setLevel(LEVEL level);

    void log(const LEVEL level, const char* file, const char* func, const int line, const int nl, const char* format, ...);
    void print(const char* format, ...);

    static Logger* getInstance() {
        if (_inst == nullptr) {
            _inst = new Logger();
        }
        return _inst;
    }

private:
    FILE* _fp;

    bool _color;

    LEVEL _level;

    std::mutex _mutex;

    static Logger* _inst;

    Logger();
    virtual ~Logger();
};

}; /*jethro*/

#define error(...)  do { jethro::Logger::getInstance()->log(jethro::Logger::LV_ERROR,   __FILE__, __FUNCTION__, __LINE__, 1, __VA_ARGS__); } while(0);
#define warn(...)   do { jethro::Logger::getInstance()->log(jethro::Logger::LV_WARNING, __FILE__, __FUNCTION__, __LINE__, 1, __VA_ARGS__); } while(0);
#define debug(...)  do { jethro::Logger::getInstance()->log(jethro::Logger::LV_DEBUG,   __FILE__, __FUNCTION__, __LINE__, 1, __VA_ARGS__); } while(0);
#define info(...)   do { jethro::Logger::getInstance()->log(jethro::Logger::LV_INFO,    __FILE__, __FUNCTION__, __LINE__, 1, __VA_ARGS__); } while(0);
#define info2(...)  do { jethro::Logger::getInstance()->log(jethro::Logger::LV_INFO,    __FILE__, __FUNCTION__, __LINE__, 0, __VA_ARGS__); } while(0);
#define prompt(...) do { jethro::Logger::getInstance()->print(__VA_ARGS__); } while(0);

#endif /*SENDKEY_SRC_LOGGER_H*/
