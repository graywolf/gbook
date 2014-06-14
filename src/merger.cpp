#include "merger.h"
#include "macros.h"
#include "funcs.h"

#include <algorithm>

using namespace std;
using namespace gbook;

void merger::merge() {
    LOG_DEBUG("Starting the merge.")
    generate_storage_changes();
    do_merge();
    LOG_DEBUG("Merge ended.")
}

void merger::generate_storage_changes() {
    LOG_DEBUG2("Filling changes for abook.")
    abook_m->fill_storage_changes(abook_changes_, last_state_);
    LOG_DEBUG2("Filling changes for google.")
    google_m->fill_storage_changes(google_changes_, last_state_);
}

void merger::do_merge() {
    set_next_id_to_use();

    update_google();
    update_abook();
}

void merger::update_abook() {
    LOG_DEBUG("Updating abook.")
    for (user * u : google_changes_.new_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            if (u->custom5.empty()) {
                u->custom5 = to_string(++id_);
                google_m->update(*u);
                LOG_DEBUG2("Assigning id " << id_ << " to " << u->name)
            }
            LOG_INFO("Adding user " << u->name << " to abook")
            abook_m->add(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }

    for (user * u : google_changes_.modified_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            LOG_INFO("Updating user " << u->name << " in abook")
            abook_m->update(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }

    for (user * u : google_changes_.deleted_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            LOG_INFO("Removing user " << u->name << " from abook")
            abook_m->remove(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }
}

void merger::update_google() {
    LOG_DEBUG("Updating google.")
    for (user * u : abook_changes_.new_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            if (u->custom5.empty()) {
                u->custom5 = to_string(++id_);
                LOG_DEBUG2("Assigning id " << id_ << " to " << u->name)
            }
            LOG_INFO("Adding user " << u->name << " to google")
            google_m->add(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }

    for (user * u : abook_changes_.modified_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            LOG_INFO("Updating user " << u->name << " in google")
            google_m->update(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }

    for (user * u : abook_changes_.deleted_users) {
        if (user_synced(u)) {
            LOG_INFO("User " << u->name << " already synced from other source.")
        } else {
            LOG_INFO("Removing user " << u->name << " from google")
            google_m->remove(*u);
            synced_c5s_.push_back(u->custom5);
        }
    }
}

void merger::set_next_id_to_use() {
    id_ = abook_m->get_max_id();
    id_ = max(id_, google_m->get_max_id());
}
