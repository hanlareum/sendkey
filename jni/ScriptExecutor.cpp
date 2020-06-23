/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <unistd.h>

#include <sstream>
#include <fstream>
#include <chrono>

#include "Util.h"
#include "ScriptExecutor.h"

using namespace std;

namespace jethro {

std::shared_ptr<ScriptExecutor> ScriptExecutor::create(std::string path, std::shared_ptr<LinuxInputWriter> inputWriter) {
	if (path.empty()) {
		error("script path is empty");
		return nullptr;
	}
	std::shared_ptr<ScriptExecutor> script(new ScriptExecutor(path, inputWriter));
	if (script && script->initialize()) {
		return script;
	}
	error("fail to create script queue");
	return nullptr;
}

ScriptExecutor::ScriptExecutor(std::string path, std::shared_ptr<LinuxInputWriter> inputWriter)
	: m_interval{0.0}, m_restInterval{0}, m_loopCount{1}, m_path{path}, m_inputWriter{inputWriter} {
}

bool ScriptExecutor::initialize() {
	std::ifstream ifs(m_path);
	if (!ifs.is_open()) {
		error("fail to open execute queue -> %s", m_path.c_str());
		return false;
	}
	info("%s>> Loading...%s", jethro::color::White.c_str(), jethro::color::Reset.c_str());
	for (std::string line; std::getline(ifs, line);) {
		if (line.empty()) {
			continue;
		}

        std::size_t found = std::string::npos;
        if (jethro::startswith(line, "KEY_")
                || jethro::startswith(line, "SCRIPT")
                || jethro::startswith(line, "LOOP")
                || jethro::startswith(line, "SLEEP")) {
            found = line.find(" ");
        }
        else {
            error("unknown event -> %s", line.c_str());
            continue;
        }

        std::string first, second;
        if (found != std::string::npos) {
            first = line.substr(0, found);
            second = line.substr(found + 1, line.length() - found);
        }
        else {
            first = line;
        }

        jethro::uppercase(first);

        if (second == "") {
            info("    -> %s", first.c_str());
        }
        else {
            info("    -> %s (%s)", first.c_str(), second.c_str());
        }

        if (first == "LOOP") {
            if (second != "") {
                m_loopCount = jethro::str2long(second, 10);
            }
            if (m_loopCount < 0) {
                m_loopCount = 1;
            }
        }
        else {
            push(first, second);
        }
	}
	return true;
}

void ScriptExecutor::run() {
	auto start = chrono::steady_clock::now();
	for (unsigned long i = 0L; (i < (unsigned long) m_loopCount) || (m_loopCount == 0L); i++) {
		info("%s>> sendkey loop=[%ld]%s", jethro::color::LightGreen.c_str(), i + 1, jethro::color::Reset.c_str());
		for (auto iter = first(); hasMore(); iter = next()) {
			std::string name = iter->first;
			std::string second = iter->second;

			if (second != "") {
				info("    -> %s (%s)", name.c_str(), second.c_str());
			} else {
				info("    -> %s", name.c_str());
			}

			if (name == "SLEEP") {
				usleep(jethro::str2long(second, 10) * 1000 * 1000);
			} else if (name == "SCRIPT") {
				std::string command = second;
				::system(command.c_str());
				usleep((m_interval > 0 ? m_interval : 0.5) * 1000 * 1000);
			} else {
				if (m_inputWriter != nullptr) {
					m_inputWriter->exec(name, second);
				}
				if (m_interval > 0) {
					usleep(m_interval * 1000 * 1000);
				}
			}
		}
		auto end = chrono::steady_clock::now();

		unsigned sec = chrono::duration_cast<chrono::seconds>(end - start).count();
		unsigned day = 0, hour = 0, min = 0;
		if (sec > 86400) {
			day = sec / 86400;
			sec = sec % 86400;
		}
		if (sec > 3600) {
			hour = sec / 3600;
			sec = sec % 3600;
		}
		if (sec > 60) {
			min = sec / 60;
			sec = sec % 60;
		}
		info("%s    -> %u/%02u:%02u:%02u [%s] elapsed.%s"
            , jethro::color::Brown.c_str()
            , day
            , hour
            , min
            , sec
            , jethro::getTimeNow("%Y.%m.%d %X").c_str()
            , jethro::color::Reset.c_str());

		if (m_restInterval > 0) {
			usleep(m_restInterval * 1000 * 1000);
		}
	}
}


} /*jethro*/
