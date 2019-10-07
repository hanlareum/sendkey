/* 

 Copyright 2018 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include "Util.h"
#include "Console.h"
#include "Database.h"
#include "LinuxInput.h"

using namespace std;

//#define CONSILE_TEST_MODE

int main(int argc, char *argv[])
{
	jethro::intro(argv[0]);
#if 0
	info("%sLightCyan%s", jethro::color::LightCyan.c_str(), jethro::color::Reset.c_str());
	info("%sLightGreen%s", jethro::color::LightGreen.c_str(), jethro::color::Reset.c_str());
	info("%sLightPurple%s", jethro::color::LightPurple.c_str(), jethro::color::Reset.c_str());
	info("%sDarkGray%s", jethro::color::DarkGray.c_str(), jethro::color::Reset.c_str());
	info("%sLightGray%s", jethro::color::LightGray.c_str(), jethro::color::Reset.c_str());
	info("%sWhite%s", jethro::color::White.c_str(), jethro::color::Reset.c_str());
	info("%sYellow%s", jethro::color::Yellow.c_str(), jethro::color::Reset.c_str());
	info("%sBrown%s", jethro::color::Brown.c_str(), jethro::color::Reset.c_str());
#endif
	std::string dbPath = "";
	if (argc > 2) {
		dbPath = argv[1];
	}

#ifndef CONSILE_TEST_MODE
	auto db = jethro::Database::create(dbPath);
	if (db == nullptr) {
		error("Fail to create database");
		return -1;
	}
	auto device = jethro::LinuxInput::create("jethro virtual remote control", "/dev/uinput", db);
	if (device == nullptr) {
		error("Fail to create LinuxInput");
		return -1;
	}
#endif

	std::string promptHeader = "";
	promptHeader += jethro::color::LightCyan;
	promptHeader += " KEY";
	promptHeader += jethro::color::Reset;
	promptHeader += "$ ";

	jethro::Console console(promptHeader);
	while (true) {

		prompt("%s KEY%s$ ", jethro::color::LightCyan.c_str(), jethro::color::Reset.c_str());

		std::string input = console.readLine();
		if (input != "") {
			jethro::uppercase(input);

			if (jethro::startswith(input, "KEY_") == false) {
				input = "KEY_" + input;
			}
#ifndef CONSILE_TEST_MODE
			device->emitKey(input);
#else
			cout << input << endl;			
#endif
		}
	}

	return 0;
}
