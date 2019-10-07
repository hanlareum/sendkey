/* 

 Copyright 2018-2019 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <string>

#include <unistd.h>
#include <signal.h>

#include "Util.h"
#include "Database.h"
#include "LinuxEvent.h"
#include "ScriptExecutor.h"

using namespace std;

static FILE *fp = NULL;

#define KEY_WRITE(...) do {\
	if (fp != NULL) {\
		fprintf(fp, __VA_ARGS__); fprintf(fp, "\n"); fflush(fp); fsync(fileno(fp)); \
	}\
	info(__VA_ARGS__); \
} while(0)

void sig_handler(int signum)
{
	KEY_WRITE("SLEEP 2");
	exit (0);
}

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

	fp = fopen("script.txt", "w");
	signal(SIGINT, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGTSTP, sig_handler);
	signal(SIGQUIT, sig_handler);

	KEY_WRITE("LOOP 0");

	long code, value;
	auto t1 = chrono::steady_clock::now();
	while(1) {
		if (device->read(code, value)) {
			auto t2 = chrono::steady_clock::now();
			unsigned sec = chrono::duration_cast<chrono::seconds>(t2 - t1).count();
			if (sec > 0) {
				KEY_WRITE("SLEEP %u", sec);

			}
			KEY_WRITE("%s %ld", device->getNameByCode(code).c_str(), value);
			t1 = t2;
		}
	}
	return 0;
}
