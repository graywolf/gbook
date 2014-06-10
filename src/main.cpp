#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <argp.h>
#include <stdexcept>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>

#include "sync.h"
#include "typedefs.h"
#include "logger.h"
#include "macros.h"
#include "config.h"

using namespace std;

enum class commands { sync = 's', manage = 'm' };

commands command;
bool verbose = false;
bool debug = false;
bool ultra_debug = false;
bool quiet = false;
string config_file;

static int parse_opt(int key, char * arg, argp_state * state) {
    switch (key) {
        case 's':
            command = commands::sync;
            break;
        case 'm':
            command = commands::manage;
            break;
        case 'v':
            verbose = true;
            break;
        case 'd':
            debug = true;
            break;
        case 'D':
            ultra_debug = true;
            break;
        case 'q':
            quiet = true;
            break;
        case 'c':
            config_file = string(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
            break;
    }
    return 0;
}

argp_option options[] = {
    {0, 0, 0, 0, "Commands"},
    {"synchronize", 's', 0, 0, "Synchronize contacts. Default if none command specified."},
    {"manage", 'm', 0, 0, "Manage contacts using abook"},
    {0, 0, 0, 0, "Options"},
    {"config-file", 'c', "CONFIG-FILE", 0, "Where is config file? Default: ~/.config/gbook"},
    {"verbose", 'v', 0, 0, "Enable some debug information"},
    {"debug", 'd', 0, 0, "Enable more debug information"},
    {"Debug", 'D', 0, 0, "Enable all debug information"},
    {"quiet", 'q', 0, 0, "Run quiet. Has priority over v & d."},
    {0}
};

const char *argp_program_version = MAJOR_VERSION "." MINOR_VERSION "." REVISION;
const char *argp_program_bug_address = "<paladin@jstation.cz>";

void handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char **argv) {
    try {
        ios_base::sync_with_stdio(false);
        locale loc ("");
        locale::global (loc);
        signal(SIGSEGV, handler);

        jstation::logger * cout_logger = new jstation::cout_logger();
        cout_logger->set_threshold(jstation::severity::INFO);
        jstation::logger_collection::instance().add_logger(cout_logger);

        config_file = getenv("HOME");
        config_file.append("/.config/gbook");

        command = commands::sync;

        argp argp = { options, parse_opt, 0, "Synchronization tool between google contacts & abook data file."};
        argp_parse(&argp, argc, argv, 0, 0, 0);

        if (verbose) {
            cout_logger->set_threshold(jstation::severity::DEBUG);
        }
        if (debug) {
            cout_logger->set_threshold(jstation::severity::DEBUG2);
        }
        if (ultra_debug) {
            cout_logger->set_threshold(jstation::severity::DEBUG3);
        }
        if (quiet) {
            cout_logger->set_threshold(jstation::severity::ERROR);
        }

        gbook::config::get(config_file);

        switch (command) {
            case commands::manage:
                LOG_INFO("Starting abook...")
                system("abook");
                LOG_INFO("Abook finished.")
                break;
            case commands::sync:
                LOG_INFO("Starting sync")
                (gbook::sync()).do_sync();
                LOG_INFO("Sync finished.")
                break;
        }
    } catch (exception & e) {
        LOG_ERROR(e.what());
    }


    return EXIT_SUCCESS;
}
