/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>
#include <unistd.h>

#include "Util.h"
#include "Database.h"
#include "LinuxEvent.h"
#include "ScriptExecutor.h"

using namespace std;

int main(int argc, char* argv[])
{
	jethro::intro(argv[0]);

	std::string path = "";

	if (argc < 2) {
		info("\n[ERROR] no device file!!\n");
		info("\nUsage : %s /dev/input/event${N}\n", argv[0]);
		return -1;
	}

	path = argv[1];

	auto db = jethro::Database::create();
	if (db == nullptr) {
		error("Fail to create Database");
		return -1;
	}
	auto device = jethro::LinuxEvent::create(path.c_str(), db);
	if (device == nullptr) {
		error("Fail to create LinuxEvent");
		return -1;
	}

	long code, value;
	while(1) {
		if (device->read(code, value)) {
#if 0
			std::string state = "";
			switch(value) {
				case 0: state = "release"; break;
				case 1: state = "press";   break;
				case 2: state = "repeat";  break;
			}
			info("%s [%s]", device->getNameByCode(code).c_str(), state.c_str());
#else
			info("%s (%u) [%u]", device->getNameByCode(code).c_str(), code, value);
#endif
		}
	}
	info(">> finished");
	return 0;
}
