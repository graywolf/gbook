#ifndef SYNC_H
#define SYNC_H

#include <string>
#include <map>
#include <set>
#include <iostream>

#include "user.h"
#include "oauth2.h"

namespace gbook {
    class sync {
    public:
        sync(std::string abook_folder) :
            abook_folder_(abook_folder),
            abook_data_file_(abook_folder + "/addressbook"),
            gbook_sync_record_(abook_folder + "/gbook_sync_record"),
            gbook_id_file_(abook_folder + "/gbook_id_file"),
            max_id_(0LL),
            o2_(abook_folder + "/o2_auth")
            {
                if (o2_.access_token().empty()) {
                    user_data ud = o2_.request_user_code();
                    std::cout << "User code: " << ud.user_code << std::endl;
                    std::cout << "Verification url: " << ud.verification_url << std::endl;
                    o2_.begin_polling();
                    std::cout << "Ok, we got access token." << std::endl;
                } else {
                    o2_.refresh_access_token();
                }
            }
        void do_sync();
    private:
        std::string abook_folder_;
        std::string abook_data_file_;
        std::string gbook_sync_record_;
        std::string gbook_id_file_;

        std::map<std::string, user> google_id_map_;
        std::map<std::string, user> abook_id_map_;
        std::map<std::string, user> abook_id_map_from_last_sync_;
        std::set<std::string> ids_changed_in_abook_;
        long long max_id_;
        std::map<std::string, std::string> abook_google_id_map_;
        std::map<std::string, std::string> google_abook_id_map_;


        oauth2 o2_;

        void load_abook();
        void load_google();
        void load_id_maps();

        void abook_to_google();
        void google_to_abook();

        void wrap_it_up();
    };
}

#endif // SYNC_H
