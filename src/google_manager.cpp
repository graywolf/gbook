#include "google_manager.h"
#include "macros.h"
#include "funcs.h"

#include <string>

using namespace std;
using namespace gbook;

void google_manager::load() {
    all_ = contacts_.get_all();
}

void google_manager::add(user & u) {
    contacts_.add(u);
}

void google_manager::update(user & u) {
    auto it = find_user_in(u, all_);
    u.set_id("google", it->get_id("google"));
    contacts_.update(u);
}

void google_manager::remove(user & u) {
    auto it = find_user_in(u, all_);
    u.set_id("google", it->get_id("google"));
    contacts_.remove(u.get_id("google"));
}

void google_manager::fill_storage_changes(storage_changes & changes, user_list & last_sync) {
    changes.new_users.clear();
    changes.modified_users.clear();
    changes.deleted_users.clear();

    for (user & u : all_) {
        if (u.custom5.empty() || !is_user_in(u, last_sync)) {
            changes.new_users.push_back(&u);
        }
    }

    for (user & u : last_sync) {
        auto it = find_user_in(u, all_);
        if (end(all_) == it) {
            changes.deleted_users.push_back(&u);
        } else if (u != *it) {
            changes.modified_users.push_back(&*it);
        }
    }
}

int google_manager::get_max_id() {
    int id = -1;
    for (user & u : all_) {
        if (!u.custom5.empty()) {
            id = max(id, stoi(u.custom5));
        }
    }
    LOG_DEBUG2("Max id from google: " << id)
    return id;
}

string google_manager::name() {
    return "google_manager";
}
