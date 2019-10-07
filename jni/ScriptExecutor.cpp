/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

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
	: m_interval{0.5}, m_restInterval{0}, m_loopCount{1}, m_path{path}, m_inputWriter{inputWriter} {
}

bool ScriptExecutor::initialize() {
	std::ifstream ifs(m_path);
	if (!ifs.is_open()) {
		error("fail to open execute queue -> %s", m_path.c_str());
		return false;
	}
	info("%s>> loading...%s", jethro::color::White.c_str(), jethro::color::Reset.c_str());
	for (std::string line; std::getline(ifs, line);) {
		if (line.empty()) {
			continue;
		}
		std::vector<std::string> data;
		int count = jethro::split(line, ' ', data);

		std::string first = data[0];
		std::string second = (count == 1) ? "" : data[1];
		jethro::uppercase(first);
		//debug("count : %d, data[0] : %s", count, data[0].c_str());

		if (count == 2) {
			info("    -> %s (%s)", first.c_str(), second.c_str());
		} else {
			info("    -> %s", first.c_str());
		}

		if (first == "INTERVAL") {
			if (count == 2) {
				m_interval = jethro::str2double(second);
			} else {
				error("INTERVAL can't set!!");
			}
		} else if (first == "REST") {
			if (count == 2) {
				m_restInterval = jethro::str2double(second);
			} else {
				error("REST can't set!!");
			}
		} else if (first == "LOOP") {
			if (count == 2) {
				m_loopCount = jethro::str2long(second, 10);
				if (m_loopCount < 0) {
					m_loopCount = 1;
				}
			} else {
				error("LOOP can't set!!");
			}
		} else if (jethro::startswith(first, "KEY_") || first == "SLEEP" || first == "WAIT" || first == "KILL") {
			push(first, second);
		} else {
			error("unknown event -> %s", first.c_str());
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
			} else if (name == "WAIT") {
				getchar();
			} else if (name == "KILL") {
				std::string command = "kill $(pidof ";
				command += second;
				command += ")";
				::system(command.c_str());
				usleep(m_interval * 1000 * 1000);
			} else {
				if (m_inputWriter != nullptr) {
					m_inputWriter->exec(name);
				}
				usleep(m_interval * 1000 * 1000);
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
		info("%s    -> %u/%02u:%02u:%02u elapsed.%s", jethro::color::Brown.c_str(), day, hour, min, sec, jethro::color::Reset.c_str());
		if (m_restInterval > 0) {
			usleep(m_restInterval * 1000 * 1000);
		}
	}
}


} /*jethro*/
