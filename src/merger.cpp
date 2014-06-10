#include "merger.h"
#include "macros.h"

#include <algorithm>

using namespace std;
using namespace gbook;

void merger::merge() {
    LOG_DEBUG("Starting the merge.")
    generate_storage_changes();
    fill_secondary_changes();
    deduplicate_changes();
    do_merge();
    LOG_DEBUG("Merge ended.")
}

void merger::generate_storage_changes() {
    LOG_DEBUG2("Filling changes for primary.")
    primary_.manager_->fill_storage_changes(primary_.changes_, last_state_);
    for (auto & manager : secondaries_) {
        LOG_DEBUG2("Filling changes for one of the secondaries.")
        manager.manager_->fill_storage_changes(manager.changes_, last_state_);
    }
}

void merger::deduplicate_changes() {
    LOG_DEBUG2("Deduplicating changes.")

    // TODO: clear ids in primary from secondaries
}

void merger::do_merge() {
    int id = get_next_id_to_use();

    update_primary(id);
    for (auto & secondary : secondaries_) {
        update_secondary(secondary, id);
    }
}

void merger::update_primary(int & id) {
    LOG_DEBUG("Processing secondaries.")
    for (user * u : secondary_changes_.new_users) {
        if (u->custom5.empty()) {
            u->custom5 = to_string(++id);
            LOG_DEBUG2("Assigning id " << id << " to " << u->name)
        }
        LOG_INFO("Adding user " << u->name << " to " << primary_.manager_->name())
        primary_.manager_->add(*u);
    }

    for (user * u : secondary_changes_.modified_users) {
        LOG_INFO("Updating user " << u->name << " in " << primary_.manager_->name())
        primary_.manager_->update(*u);
    }

    for (user * u : secondary_changes_.deleted_users) {
        LOG_INFO("Removing user " << u->name << " from " << primary_.manager_->name())
        primary_.manager_->remove(u->custom5);
    }
}

void merger::update_secondary(manager_record & secondary, int & id) {
    LOG_DEBUG("Updating secondary: " << secondary.manager_->name())
    for (user * u : primary_.changes_.new_users) {
        if (u->custom5.empty()) {
            u->custom5 = to_string(++id);
            LOG_DEBUG2("Assigning id " << id << " to " << u->name)
        }
        LOG_INFO("Adding user " << u->name << " to " << secondary.manager_->name())
        secondary.manager_->add(*u);
    }

    for (user * u : primary_.changes_.modified_users) {
        LOG_INFO("Updating user " << u->name << " in " << secondary.manager_->name())
        secondary.manager_->update(*u);
    }

    for (user * u : primary_.changes_.deleted_users) {
        LOG_INFO("Removing user " << u->name << " from " << secondary.manager_->name())
        secondary.manager_->remove(u->custom5);
    }
}

void merger::fill_secondary_changes() {
    for (auto & manager : secondaries_) {
        LOG_DEBUG2("Filling secondary changes from " << manager.manager_->name())
        for (user * u : manager.changes_.new_users) {
            if (
                find_if(
                    secondary_changes_.new_users.begin(),
                    secondary_changes_.new_users.end(),
                    [u] (const user * in_vector) { return *u == *in_vector; }
                )
                ==
                secondary_changes_.new_users.end()
            ) {
                LOG_DEBUG2("New user in secondaries: " << u->name)
                secondary_changes_.new_users.push_back(u);
            }
        }

        for (user * u : manager.changes_.modified_users) {
            if (
                find_if(
                    secondary_changes_.modified_users.begin(),
                    secondary_changes_.modified_users.end(),
                    [u] (const user * in_vector) { return *u == *in_vector; }
                )
                ==
                secondary_changes_.modified_users.end()
            ) {
                LOG_DEBUG2("Modified user in secondaries: " << u->name)
                secondary_changes_.modified_users.push_back(u);
            }
        }

        for (user * u : manager.changes_.deleted_users) {
            if (
                find_if(
                    secondary_changes_.deleted_users.begin(),
                    secondary_changes_.deleted_users.end(),
                    [u] (const user * in_vector) { return *u == *in_vector; }
                )
                ==
                secondary_changes_.deleted_users.end()
            ) {
                LOG_DEBUG2("Deleted user in secondaries: " << u->name)
                secondary_changes_.deleted_users.push_back(u);
            }
        }
    }
}

int merger::get_next_id_to_use() {
    int id = primary_.manager_->get_max_id();
    for (auto & storage : secondaries_) {
        id = max(id, storage.manager_->get_max_id());
    }
    return id;
}

merger::~merger() {
    if (primary_.owner_) {
        delete primary_.manager_;
    }
    for (auto & manager : secondaries_) {
        if (manager.owner_) {
            delete manager.manager_;
        }
    }
}
