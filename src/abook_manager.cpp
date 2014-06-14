#include "abook_manager.h"
#include "macros.h"
#include "funcs.h"

#include <string>

using namespace std;
using namespace gbook;

void abook_manager::load(std::istream & current_stream) {
    all_ = load_users_from_stream(current_stream);

    for (user & uptr : all_) {
        current_.push_back(&uptr);
    }
}

void abook_manager::flush(ostream & stream) const {
    stream << current_;
}

void abook_manager::add(user & u) {
    if (is_user_in(u, current_)) {
        throw invalid_argument("User with this custom5 already exists.");
    }
    all_.push_back(u);
    current_.push_back(&all_.back());
}

void abook_manager::update(user & u) {
    auto it = find_user_in(u, all_);
    if (end(all_) == it) {
        throw invalid_argument("User with this custom5 doesn't exist.");
    }
    *it = u;
}

void abook_manager::remove(user & u) {
    auto it = find_user_in(u, current_);
    if (end(current_) == it) {
        LOG_INFO("User with this custom5 doesn't exist: " << u.custom5);
    } else {
        current_.erase(it);
    }
}

void abook_manager::fill_storage_changes(storage_changes & changes, user_list & last_sync) {
    changes.new_users.clear();
    changes.modified_users.clear();
    changes.deleted_users.clear();

    for (user * u : current_) {
        if (u->custom5.empty() || !is_user_in(u, current_)) {
            changes.new_users.push_back(u);
        }
    }

    for (user & u : last_sync) {
        auto cur_it = find_user_in(u, current_);
        if (end(current_) == cur_it) {
            changes.deleted_users.push_back(&u);
        } else if (u != **cur_it) {
            changes.modified_users.push_back(*cur_it);
        }
    }
}

int abook_manager::get_max_id() {
    int id = -1;
    for (user * u : current_) {
        if (!u->custom5.empty()) {
            id = max(id, stoi(u->custom5));
        }
    }
    LOG_DEBUG2("Max id from abook: " << id)
    return id;
}

string abook_manager::name() {
    return "abook_manager";
}
