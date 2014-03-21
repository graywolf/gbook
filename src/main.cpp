#include <iostream>
#include <stdlib.h>
#include <cstdlib>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include "../include/config.h"

using namespace std;
namespace po = boost::program_options;

po::options_description o_commands("Commands");

po::options_description o_sync("Sync");
enum class sync_types { pull, push, both };

po::options_description o_add("Add");
po::options_description o_manage("Manage");

po::options_description o_general("General");

po::options_description o_combined;

po::variables_map vm;

istream& operator>>(istream& in, sync_types& sync_type) {
    std::string token;
    in >> token;
    if (boost::algorithm::iequals(token, "pull")) {
        sync_type = sync_types::pull;
    } else if (boost::algorithm::iequals(token, "push")) {
        sync_type = sync_types::push;
    } else if (boost::algorithm::iequals(token, "both")) {
        sync_type = sync_types::both;
    } else {
        throw po::validation_error(po::validation_error::invalid_option_value, "sync-direction", token);
    }
    return in;
}

ostream& operator<<(ostream& out, sync_types& sync_type) {
    switch(sync_type) {
        case sync_types::both:
            cout << "both";
            break;
        case sync_types::pull:
            cout << "pull";
            break;
        case sync_types::push:
            cout << "push";
            break;
    }
    return out;
}

void declare_program_options() {
    o_commands.add_options()
        ("sync,s", "synchronize contacts with backend")
        ("add,a", "add new contact")
        ("manage,m", "manage contacts using abook")
        ("help,h", "prints help message")
        ("version", "prints version information")
    ;

    o_sync.add_options()
        (
            "sync-direction", po::value<sync_types>()->default_value(sync_types::both, "both"),
            "In which direction do the synchronization, posible values are:\n\n"
            "  both: \ttwo-way synchronization\n"
            "  push: \tpush local changes to the server\n"
            "  pull: \tpull changes from servet"
        )
    ;

    o_combined.add(o_commands).add(o_sync).add(o_add).add(o_manage).add(o_general);
}

void parse(int argc, char ** argv) {
    po::store(po::parse_command_line(argc, argv, o_combined), vm);
    po::notify(vm);
}

void print_help(char ** argv) {
    cout << "Usage: " << argv[0] << " [command] [options]" << endl << endl;
    cout << "Each command can have some modifiers described in it's own section of this help message." << endl << endl;
    cout << o_commands << endl;
    cout << o_sync << endl;
    cout << o_add << endl;
    cout << o_manage << endl;
    cout << o_general << endl;
}

void print_version() {
    cout << "gBook version " << MAJOR_VERSION << "." << MINOR_VERSION << "." << REVISION << endl;
}

int main(int argc, char **argv) {
    ios_base::sync_with_stdio(false);
    locale loc ("");
    locale::global (loc);

    declare_program_options();
    parse(argc, argv);

    if (vm.count("version")) {
        print_version();
    } else if (vm.count("manage")) {
        system("abook");
    } else {
        print_help(argv);
    }

    return EXIT_SUCCESS;
}
