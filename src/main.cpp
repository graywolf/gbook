#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <argp.h>
#include <stdexcept>

#include "../include/config.h"

#include "sync.h"

using namespace std;

enum class commands { sync = 's', manage = 'm' };

commands command;

static int parse_opt(int key, char * arg, argp_state * state) {
    switch (key) {
        case 's':
            command = commands::sync;
            break;
        case 'm':
            command = commands::manage;
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
    {0}
};

const char *argp_program_version = MAJOR_VERSION "." MINOR_VERSION "." REVISION;
const char *argp_program_bug_address = "<paladin@jstation.cz>";

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    locale loc ("");
    locale::global (loc);

    command = commands::sync;

    argp argp = { options, parse_opt, 0, "Synchronization tool between google contacts & abook data file."};
    argp_parse(&argp, argc, argv, 0, 0, 0);

    switch (command) {
        case commands::manage:
            system("abook");
            break;
        case commands::sync:
            try {
                string abook_dir = getenv("HOME");
                abook_dir.append("/.abook");
                gbook::sync s(abook_dir);
                s.do_sync();
            } catch (runtime_error re) {
                cout << "Exception was caught: " << re.what() << endl;
            }
            break;
    }

    return EXIT_SUCCESS;
}
