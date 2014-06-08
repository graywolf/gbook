#include "abook_manager.h"

#include <sstream>

using namespace std;
using namespace gbook;

void abook_manager::load(std::istream & current_stream) {
    current_ = load_users_from_stream(current_stream);
}

void abook_manager::flush(ostream stream) const {
    stream << current_;
}

int abook_manager::find_user_id_by_custom5(std::string id) {
    int index = -1;
    for (size_t i = 0; i < current_.size(); ++i) {
        if (current_.at(i).custom5 == id) {
            index = i;
            break;
        }
    }
    return index;
}

void abook_manager::add(user & u) {
    if (find_user_id_by_custom5(u.custom5) != -1) {
        throw invalid_argument("User with this custom5 already exists.");
    }
    current_.push_back(u);
}

void abook_manager::update(user & u) {
    int i = find_user_id_by_custom5(u.custom5);
    if (i == -1) {
        throw invalid_argument("User with this custom5 doesn't exist.");
    }
    current_[i] = u;
}

void abook_manager::remove(std::string id) {
    int i = find_user_id_by_custom5(id);
    if (i == -1) {
        throw invalid_argument("User with this custom5 doesn't exist.");
    }
    current_.erase(begin(current_) + i);
}

void abook_manager::fill_storage_changes(storage_changes & changes, user_list & last_sync) {
    changes.new_users.clear();
    for (size_t i = 0; i < current_.size(); ++i) {
        if (current_.at(i).custom5.empty()) {
            changes.new_users.push_back(&current_.at(i));
        }
    }

    changes.modified_users.clear();
    changes.deleted_users.clear();
    for (size_t i = 0; i < last_sync.size(); ++i) {
        int current_id = find_user_id_by_custom5(last_sync.at(i).custom5);
        if (current_id == -1) {
            changes.deleted_users.push_back(last_sync.at(i));
        } else if (last_sync.at(i) != current_.at(current_id)) {
            changes.modified_users.push_back(&current_.at(current_id));
        }
    }
}
