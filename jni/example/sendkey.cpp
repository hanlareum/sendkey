/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <unistd.h>

#include "Util.h"
#include "Database.h"
#include "LinuxInput.h"
#include "LinuxInputWriter.h"
#include "ScriptExecutor.h"

int main(int argc, char* argv[])
{
	jethro::intro(argv[0]);

	char *filename = (char *) "script.txt";
	if (argc > 1) {
		filename = argv[1];
	}

	auto db = jethro::Database::create("db.dat");
	if (db == nullptr) {
		error("Fail to create Database");
		return -1;
	}
	auto device = jethro::LinuxInput::create("virtual remote control", "/dev/uinput", db);
	if (device == nullptr) {
		error("Fail to create LinuxInput");
		return -1;
	}

	auto sender = jethro::LinuxInputWriter::create(device);
	if (sender == nullptr) {
		error("Fail to create LinuxInputWriter");
		return -1;
	}

	usleep(100 * 1000);

	auto script = jethro::ScriptExecutor::create(filename, sender);
	if (script == nullptr) {
		error("Fail to create ScriptExecutor");
		return -1;
	}

	info("%s>> Press any key to start sendkey - (%s) %s", jethro::color::LightPurple.c_str(), filename, jethro::color::Reset.c_str());
	getchar();

	script->run();

	info(">> finished");
	return 0;
}
