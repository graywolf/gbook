#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <argp.h>
#include <stdexcept>

#include "../include/config.h"

#include "sync.h"
#include "typedefs.h"
#include "logger.h"
#include "macros.h"

using namespace std;

enum class commands { sync = 's', manage = 'm' };

commands command;
bool verbose = false;
bool debug = false;
bool quiet = false;

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
        case 'q':
            quiet = true;
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
    {"verbose", 'v', 0, 0, "Enable some debug information"},
    {"debug", 'd', 0, 0, "Enable all debug information"},
    {"quiet", 'q', 0, 0, "Run quiet. Has priority over v & d."},
    {0}
};

const char *argp_program_version = MAJOR_VERSION "." MINOR_VERSION "." REVISION;
const char *argp_program_bug_address = "<paladin@jstation.cz>";

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    locale loc ("");
    locale::global (loc);

    jstation::logger * cout_logger = new jstation::cout_logger();
    cout_logger->set_threshold(jstation::severity::INFO);
    jstation::logger_collection::instance().add_logger(cout_logger);

    command = commands::sync;

    argp argp = { options, parse_opt, 0, "Synchronization tool between google contacts & abook data file."};
    argp_parse(&argp, argc, argv, 0, 0, 0);

    if (verbose) {
        cout_logger->set_threshold(jstation::severity::DEBUG);
    }
    if (debug) {
        cout_logger->set_threshold(jstation::severity::DEBUG3);
    }
    if (quiet) {
        cout_logger->set_threshold(jstation::severity::ERROR);
    }

    switch (command) {
        case commands::manage:
            LOG_INFO("Starting abook...")
            system("abook");
            LOG_INFO("Abook finished.")
            break;
        case commands::sync:
            try {
                LOG_INFO("Starting sync")
                string abook_dir = getenv("HOME");
                abook_dir.append("/.abook");
                LOG_DEBUG("Abook directory determined as: " << abook_dir)
                gbook::sync s(abook_dir);
                s.do_sync();
                LOG_INFO("Sync finished.")
            } catch (runtime_error re) {
                LOG_ERROR(re.what());
            } catch (invalid_argument re) {
                LOG_ERROR(re.what());
            }
            break;
    }

    return EXIT_SUCCESS;
}
