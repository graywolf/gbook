#include "sync.h"
#include "user_reader.h"
#include "user_writer.h"
#include "contacts.h"
#include "config.h"
#include "macros.h"
#include "abook_manager.h"
#include "google_manager.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace gbook;

sync::sync() : m_(synced_c5s_) {
    ensure_token();
    am_ = new abook_manager();
    gm_ = new google_manager();
}

sync::~sync() {
    delete am_;
    delete gm_;
}

void sync::do_sync() {
    load_managers();
    m_.merge();
    flush_managers();
}

void sync::ensure_token() {
    if (config::access_token().empty()) {
        LOG_DEBUG("Access token is empty, contacting server...");
        string verification_url, user_code;
        o2_.request_user_code(verification_url, user_code);
        cout    << "To continue please go to:\n    "
                << verification_url
                << "\nand enter the following code when prompted to:\n    "
                << user_code
                << endl;
        o2_.begin_polling();
        LOG_DEBUG("Ok, we've got access token.")
    } else {
        LOG_DEBUG("Refreshing access token.");
        o2_.refresh_access_token();
    }
}

void sync::load_managers() {
    load_last_state();
    load_abook();
    load_google();
}

void sync::flush_managers() {
    ofstream last_state(config::last_state_file());
    am_->flush(last_state);
    ofstream address_book(config::abook_file());
    am_->flush(address_book);
}

void sync::load_last_state() {
    LOG_DEBUG("Loading last state file " << config::last_state_file());
    ifstream last_state_stream(config::last_state_file());
    m_.set_last_state(load_users_from_stream(last_state_stream));
    LOG_DEBUG2("Last state file loaded.");
}

void sync::load_abook() {
    LOG_DEBUG("Creating abook manager.");
    LOG_DEBUG("Loading abook manager from " << config::abook_file());
    ifstream abook_istream(config::abook_file());
    am_->load(abook_istream);
    m_.abook_m = am_;
    LOG_DEBUG("Abook manager creation finished.");
}

void sync::load_google() {
    LOG_DEBUG("Creating google manager.");
    LOG_DEBUG("Loading google manager");
    gm_->load();
    m_.google_m = gm_;
    LOG_DEBUG("Google manager creation finished.");
}
