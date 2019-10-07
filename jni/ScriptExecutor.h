/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <list>
#include <string>
#include <memory>

#include "Queue.h"
#include "LinuxInputWriter.h"

#ifndef SENDKEY_SRC_SCRIPT_EXECUTER_H
#define SENDKEY_SRC_SCRIPT_EXECUTER_H

namespace jethro {

class ScriptExecutor : public jethro::Queue {
public:
	static std::shared_ptr<ScriptExecutor> create(std::string path, std::shared_ptr<LinuxInputWriter> inputWriter);

	double getInterval() {
		return m_interval;
	}
	unsigned int getLoopCount() {
		return m_loopCount;
	}

	void run();

private:
	ScriptExecutor(std::string path, std::shared_ptr<LinuxInputWriter> inputWriter);

	bool initialize();

	double m_interval;
	double m_restInterval;
	long m_loopCount;

	std::string m_path;

	std::shared_ptr<LinuxInputWriter> m_inputWriter;
}; /*ScriptExecutor*/

} /*jethro*/

#endif /*SENDKEY_SRC_SCRIPT_EXECUTER_H*/
