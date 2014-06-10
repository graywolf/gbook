#include "google_manager.h"
#include "macros.h"

#include <sstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace gbook;

void google_manager::load() {
    all_ = contacts_.get_all();
}

int google_manager::find_user_id_by_custom5(std::string id) {
    int index = -1;
    for (size_t i = 0; i < all_.size(); ++i) {
        if (all_.at(i).custom5 == id) {
            index = i;
            break;
        }
    }
    return index;
}

int google_manager::find_user_id_by_custom5(user_list & haystack, std::string id) {
    int index = -1;
    for (size_t i = 0; i < haystack.size(); ++i) {
        if (haystack.at(i).custom5 == id) {
            index = i;
            break;
        }
    }
    return index;
}

void google_manager::add(user & u) {
    contacts_.add(u);
}

void google_manager::update(user & u) {
    contacts_.update(u);
}

void google_manager::remove(std::string id) {
    for (size_t i = 0; i < all_.size(); ++i) {
        if (all_.at(i).custom5 == id) {
            contacts_.remove(all_.at(i).get_id("google"));
            break;
        }
    }
}

void google_manager::fill_storage_changes(storage_changes & changes, user_list & last_sync) {
    changes.new_users.clear();
    for (size_t i = 0; i < all_.size(); ++i) {
        if (all_.at(i).custom5.empty() || find_user_id_by_custom5(last_sync, all_.at(i).custom5) == -1) {
            changes.new_users.push_back(&all_.at(i));
        }
    }

    changes.modified_users.clear();
    changes.deleted_users.clear();
    for (size_t i = 0; i < last_sync.size(); ++i) {
        int all_id = find_user_id_by_custom5(last_sync.at(i).custom5);
        if (all_id == -1) {
            changes.deleted_users.push_back(&last_sync.at(i));
        } else if (last_sync.at(i) != all_.at(all_id)) {
            changes.modified_users.push_back(&all_.at(all_id));
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
