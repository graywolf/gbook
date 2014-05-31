#include "sync.h"
#include "user_reader.h"
#include "user_writer.h"
#include "contacts.h"
#include "config.h"
#include "macros.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace gbook;

sync::sync(string abook_folder) {
    if (config::get().access_token().empty()) {
        LOG_DEBUG("Access token is empty, contacting server...");
        user_data ud = o2_.request_user_code();
        cout    << "To continue please go to "
                << ud.verification_url << endl;
                << "And enter the following code when prompted to: "
                << ud.user_code
                << endl;
        o2_.begin_polling();
        cout << "Ok, we've got access token." << endl;
    } else {
        LOG_DEBUG("Refreshing access token.");
        o2_.refresh_access_token();
    }
}

void sync::do_sync() {
}
