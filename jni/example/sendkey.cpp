/*

 Copyright 2018-2020 Jethro Kwon (hanlareum@gmail.com), All Rights Reserved.

*/

#include <unistd.h>
#include <signal.h>

#include "Util.h"
#include "Database.h"
#include "LinuxInput.h"
#include "LinuxEvent.h"
#include "LinuxInputWriter.h"
#include "ScriptExecutor.h"

using namespace std;

typedef enum {
    RUNMODE_NONE=0
    , RUNMODE_SENDKEY
    , RUNMODE_INSPECT
    , RUNMODE_MAX
} runmode_t;

int doing_help(const char* pn);
int doing_inspect(const char* pn, const char* filename);
int doing_sendkey(const char* pn, const char* filename);

int main(int argc, char* argv[])
{
    int opt = -1;
    char filename[256] = {0,};
    std::string pn = argv[0];

    runmode_t runmode = RUNMODE_SENDKEY;

    sprintf(filename, "script.txt");

    while ((opt = getopt(argc, argv, "hri:")) != -1) {
        switch (opt) {
            case 'h':
                return doing_help(pn.c_str());
            case 'r':
                runmode = RUNMODE_INSPECT;
                break;
            case 'i':
                sprintf(filename, "%s", optarg);
                break;
        }
    }

    if (runmode == RUNMODE_INSPECT) {
        return doing_inspect(pn.c_str(), filename);
    }
    else if (runmode == RUNMODE_SENDKEY) {
        return doing_sendkey(pn.c_str(), filename);
    }
    return -1;
}

int doing_sendkey(const char* pn, const char* filename)
{
    jethro::intro(pn);

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
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    exit (0);
}

int doing_inspect(const char* pn, const char* filename)
{
    jethro::intro(pn);

    std::string path = filename;

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

int doing_help(const char* pn)
{
    jethro::intro(pn);
    info("");
    info("Usage : %s [OPTIONS]", pn);
    info("  -h          print usage.");
    info("  -r          inspect mode");
    info("  -i [FILE]   path of script or event device");
    info("");
    info("Examples :");
    info("  %s -r -i /dev/input/event1", pn);
    info("  %s -i /data/script.txt", pn);
    info("  %s", pn);
    info("");
    return 0;
}
