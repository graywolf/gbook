#include "sync.h"
#include "user_reader.h"
#include "user_writer.h"
#include "contacts.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace gbook;

void sync::do_sync() {
    load_id_maps();
    load_abook();
    load_google();

    abook_to_google();
    google_to_abook();

    wrap_it_up();
}

void sync::load_id_maps() {
    cout << "Loading gbook <--> abook mapping... ";
    ifstream inf(gbook_id_file_);
    string abook_id, google_id;
    while (inf >> abook_id >> google_id) {
        abook_google_id_map_[abook_id] = google_id;
        google_abook_id_map_[google_id] = abook_id;

        long long id = stoll(abook_id);
        if (id > max_id_) {
            max_id_ = id;
        }
    }
    cout << "Loaded." << endl;
}

void sync::load_abook() {
    cout << "Loading abook... ";
    ifstream inf(abook_data_file_);
    vector<user> abook_users = load_users_from_stream(inf);
    for (user & u : abook_users) {
        if (u.custom5.empty()) {
            u.custom5 = to_string(++max_id_);
        }
        abook_id_map_[u.custom5] = u;
    }
    cout << "Loaded." << endl;

    cout << "Loading records of last sync... ";
    ifstream inf_sync_record(gbook_sync_record_);
    vector<user> last_synced_users = load_users_from_stream(inf_sync_record);
    for (user & u : last_synced_users) {
        abook_id_map_from_last_sync_[u.custom5] = u;
    }
    cout << "Loaded." << endl;
}

void sync::load_google() {
    cout << "Loading contacts from google... ";
    contacts cs(o2_);
    vector<user> google_users = cs.get_all();
    for (user & u : google_users) {
        google_id_map_[u.get_id("google")] = u;
    }
    cout << "Loaded." << endl;
}

void sync::abook_to_google() {
    cout << "Sync abook --> google..." << endl;
    contacts cs(o2_);
    //let's find out deleted
    cout << "Deleting deleted..." << endl;
    for (auto & it : abook_id_map_from_last_sync_) {
        if (abook_id_map_.find(it.first) == abook_id_map_.end()) {
            try {
                cout << "Removing " << it.second.name << " from google." << endl;
                ids_changed_in_abook_.insert(it.second.custom5);
                cs.remove(abook_google_id_map_[it.second.custom5]);
                //remove it from two-way map
                google_id_map_.erase(abook_google_id_map_[it.second.custom5]);
                google_abook_id_map_.erase(abook_google_id_map_[it.second.custom5]);
                abook_google_id_map_.erase(it.second.custom5);
            } catch (runtime_error re) {
                cerr << "Cannot delete: " << re.what() << endl;
            }
        }
    }
    cout << "Done." << endl;

    //let's find out new
    cout << "Adding new..." << endl;
    for (auto & it : abook_id_map_) {
        if (abook_id_map_from_last_sync_.find(it.first) == abook_id_map_from_last_sync_.end()) {
            try {
                cout << "Adding " << it.second.name << " to google." << endl;
                cs.add(it.second);
                //add it to two-way map
                google_id_map_[it.second.get_id("google")] = it.second;
                abook_google_id_map_[it.second.custom5] = it.second.get_id("google");
                google_abook_id_map_[it.second.get_id("google")] = it.second.custom5;
            } catch (runtime_error re) {
                cerr << "Cannot add: " << re.what() << endl;
            }
        }
    }
    cout << "Done." << endl;

    //let's find out updated
    cout << "Updating updated..." << endl;
    for (auto & it : abook_id_map_) {
        //if existed in last sync
        if (abook_id_map_from_last_sync_.find(it.first) != abook_id_map_from_last_sync_.end()) {
            //check if it changed
            if (it.second != abook_id_map_from_last_sync_.find(it.first)->second) {
                try {
                    cout << "Updating " << it.second.name << " in google." << endl;
                    it.second.set_id("google", abook_google_id_map_[it.second.custom5]);
                    ids_changed_in_abook_.insert(it.second.custom5);
                    cs.update(it.second);
                } catch (runtime_error re) {
                    cerr << "Cannot update: " << re.what() << endl;
                }
            }
        }
    }
    cout << "Done." << endl;
}

void sync::google_to_abook() {
    cout << "Sync abook <-- google..." << endl;
    contacts cs(o2_);

    //let's find out deleted
    cout << "Deleting deleted..." << endl;
    for (auto & it : google_abook_id_map_) {
        //there is deleted
        if (google_id_map_.find(it.first) == google_id_map_.end()) {
            //delete from abook
            if (abook_id_map_.find(it.second) != abook_id_map_.end()) {
                cout << "Deleting " << abook_id_map_[it.second].name << " from abook." << endl;
                abook_google_id_map_.erase(it.second);
                abook_id_map_.erase(it.second);
            }
        }
    }

    cout << "Processing google contacts..." << endl;
    for (auto & it : google_id_map_) {
        //this is new contact from google
        if (google_abook_id_map_.find(it.second.get_id("google")) == google_abook_id_map_.end()) {
            cout << "Adding " << it.second.name << " to abook." << endl;
            it.second.custom5 = to_string(++max_id_);
            //push custom5 upstream
            cs.update(it.second);
            abook_id_map_[it.second.custom5] = it.second;
            //add it to two-way map
            abook_google_id_map_[it.second.custom5] = it.second.get_id("google");
            google_abook_id_map_[it.second.get_id("google")] = it.second.custom5;
        } else {
        //this contact is already in the db, test if there's need for update
            user & abook_user = abook_id_map_[google_abook_id_map_.find(it.second.get_id("google"))->second];
            if (abook_user != it.second && ids_changed_in_abook_.count(it.second.custom5) == 0) {
                cout << "Updating " << it.second.name << " in abook." << endl;
                abook_user = it.second;
            }
        }
    }
    cout << "Done." << endl;
}

void sync::wrap_it_up() {
    cout << "Updating files..." << endl;

    cout << "Saving new abook addressbook... ";
    vector<user> abook_users;
    for (auto & it : abook_id_map_) {
        abook_users.push_back(it.second);
    }
    ofstream abook_data_file(abook_data_file_);
    abook_data_file << abook_users;
    cout << "Done." << endl;

    cout << "Saving sync record... ";
    ofstream gbook_sync_record_file(gbook_sync_record_);
    gbook_sync_record_file << abook_users;
    cout << "Done." << endl;

    cout << "Saving abook <--> google mapping... ";
    ofstream gbook_id_file(gbook_id_file_);
    for (auto & it : abook_google_id_map_) {
        gbook_id_file << it.first << " " << it.second << endl;
    }
    cout << "Done." << endl;
}
