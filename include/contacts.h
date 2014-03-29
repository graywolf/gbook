#ifndef libgcon_CONTACTS_H
#define libgcon_CONTACTS_H

#include "curl.h"
#include "oauth2.h"
#include "user.h"

#include "tinyxml2/tinyxml2.h"

#include <iostream>

namespace gbook {
    class contacts {
    public:
        contacts(oauth2 o2) : oauth2_(o2) {}
        std::vector<user> get_all();
        void add(gbook::user& u);
        void update(user u);
        void remove(std::string id);
    private:
        void prepare_curl(curl & c) {
            c.header("GData-Version", "3.0");
            c.header("Authorization", std::string("Bearer ").append(oauth2_.access_token()));
        }
        oauth2 & oauth2_;
        /**
         * Maps google data user entry to gbook::user structure.
         *
         * \return std::string id
         **/
        void map_entry_to_user(tinyxml2::XMLElement * entry, user &user);
        void map_user_to_entry(gbook::user& user, tinyxml2::XMLElement* entry, tinyxml2::XMLDocument& d);
    };
}

#endif // libgcon_CONTACTS_H
