#ifndef libgcon_CONTACTS_H
#define libgcon_CONTACTS_H

#include "curl.h"
#include "oauth2.h"
#include "user.h"

#include "tinyxml2/tinyxml2.h"

#include <iostream>

namespace gbook {
    /**
     * Class representing interface to Google Contacts API.
     **/
    class contacts {
    public:
        /**
         * Constructor
         *
         * \param gbook::oauth2 o2 initialized OAuth2 instance
         **/
        contacts();
        /**
         * Gets all users from Google Contacts.
         *
         * \return std::vector<gbook::user>
         **/
        std::vector<user> get_all();
        /**
         * Adds new user to Google Contacts.
         *
         * \param gbook::user& u User to be added.
         **/
        void add(user & u);
        /**
         * Updates user in Google Contacts.
         *
         * \param gbook::user u User to be updated.
         **/
        void update(user u);
        /**
         * Removes user from Google Contacts.
         *
         * \param std::string id Id of user to be removed.
         **/
        void remove(std::string id);
    private:
        /**
         * Sets common option to curl.
         * \param gbook::curl& c Curl instance to operate on.
         **/
        void prepare_curl(curl & c) {
            c.header("GData-Version", "3.0");
            c.header("Authorization", std::string("Bearer ").append(config::access_token()));
        }
        std::string my_contacts_group_id_;
        /**
         * Maps google data user entry to gbook::user structure.
         *
         * \param tinyxml::XMLElement* entry Root element of the user entity
         * \param gbook::user& user User to fill from entity
         **/
        void map_entry_to_user(tinyxml2::XMLElement * entry, user &user);
        /**
         * Maps gbook::user structure to google data user entry.
         *
         * \param gbook::user& user User to put into entity
         * \param tinyxml::XMLElement* entry Root element of the user entity
         * \param tinyxml2::XMLDocument& d Document to use as parent of all new elements
         **/
        void map_user_to_entry(gbook::user& user, tinyxml2::XMLElement* entry, tinyxml2::XMLDocument& d);
    };
}

#endif // libgcon_CONTACTS_H
