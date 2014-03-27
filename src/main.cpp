#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <argp.h>

#include "../include/config.h"

using namespace std;

enum sync_types { pull = 2, push = 4, twoway = 8 };
enum commands { sync = 's', manage = 'm' };

sync_types sync_type;
commands command;

static int parse_opt(int key, char * arg, argp_state * state) {
    switch (key) {
        case 's':
            command = commands::sync;
            break;
        case 'm':
            command = commands::manage;
            break;
        case 't':
            sync_type = sync_types::twoway;
            break;
        case 'u':
            sync_type = sync_types::push;
            break;
        case 'd':
            sync_type = sync_types::pull;
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
    {0, 0, 0, 0, "Synchronization direction - in which direction should sync be done. Only relevant if 'synchronize' command is used."},
    {"two-way", 't', 0, 0, "two-way sync, default"},
    {"up", 'u', 0, 0, "sync local changes to google server"},
    {"down", 'd', 0, 0, "download changes from google server"},
    {0}
};

const char *argp_program_version = MAJOR_VERSION "." MINOR_VERSION "." REVISION;
const char *argp_program_bug_address = "<paladin@jstation.cz>";

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    locale loc ("");
    locale::global (loc);

    argp argp = { options, parse_opt, 0, "Synchronization tool between google contacts & abook data file."};
    argp_parse(&argp, argc, argv, 0, 0, 0);

    switch (command) {
        case commands::manage:
            system("abook");
            break;
        case commands::sync:
            break;
    }

    return EXIT_SUCCESS;
}
