#ifndef SYNC_H
#define SYNC_H

#include <string>
#include <map>
#include <set>
#include <iostream>

#include "user.h"
#include "oauth2.h"

namespace gbook {
    /**
     * Class handling syncing between abook and google contacts.
     **/
    class sync {
    public:
        /**
         * Constructs sync instance and assure we have access token. Either authenticates user
         * or refreshes access token.
         **/
        sync(std::string abook_folder) :
            abook_folder_(abook_folder),
            abook_data_file_(abook_folder + "/addressbook"),
            gbook_sync_record_(abook_folder + "/gbook_sync_record"),
            gbook_id_file_(abook_folder + "/gbook_id_file"),
            max_id_(0LL),
            o2_(abook_folder + "/o2_config")
            {
                if (o2_.access_token().empty()) {
                    std::cout << "Access token is empty, contacting server..." << std::endl;
                    user_data ud = o2_.request_user_code();
                    std::cout << "Go to: " << ud.verification_url << std::endl;
                    std::cout << "And enter code: " << ud.user_code << std::endl;
                    o2_.begin_polling();
                    std::cout << "Ok, we've got access token." << std::endl;
                } else {
                    std::cout << "Refreshing access token." << std::endl;
                    o2_.refresh_access_token();
                }
            }
        /**
         * Do the whole sync.
         **/
        void do_sync();
    private:
        std::string abook_folder_;
        std::string abook_data_file_;
        std::string gbook_sync_record_;
        std::string gbook_id_file_;

        //google users indexed by google contact id
        std::map<std::string, user> google_id_map_;
        //abook users indexed by abook id
        std::map<std::string, user> abook_id_map_;
        //users as they were after last sync indexed by abook id
        std::map<std::string, user> abook_id_map_from_last_sync_;
        //set with users changed in abook, to be updated in google contacts
        std::set<std::string> ids_changed_in_abook_;
        long long max_id_;
        //two maps mappinig abook ids to google ids and vice versa
        std::map<std::string, std::string> abook_google_id_map_;
        std::map<std::string, std::string> google_abook_id_map_;

        oauth2 o2_;

        /**
         * Loads abook data file.
         **/
        void load_abook();
        /**
         * Loads google contacts.
         **/
        void load_google();
        /**
         * Loads abook-google id mappings.
         **/
        void load_id_maps();

        /**
         * Handles changes in abook and pushes them into google.
         **/
        void abook_to_google();
        /**
         * Handles changes in google and pushes them into abook.
         **/
        void google_to_abook();

        /**
         * Save the resulting list of users into abook data file, saves id maps and cleans up.
         **/
        void wrap_it_up();
    };
}

#endif // SYNC_H
