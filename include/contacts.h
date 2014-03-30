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
        contacts(oauth2 o2) : oauth2_(o2) {
            curl c;
            prepare_curl(c);
            c.url("https://www.google.com/m8/feeds/groups/default/full");
            c.method(method::GET);
            c.execute();

            tinyxml2::XMLDocument d;
            d.Parse(c.received_body().c_str());
            if (d.Error()) {
                throw std::runtime_error(std::string("Cannot parse groups xml: ").append(c.received_body()));
            }
            tinyxml2::XMLElement * feed = d.FirstChildElement("feed");
            if (!feed) {
                throw std::runtime_error(std::string("Cannot find feed element: ").append(c.received_body()));
            }
            for (tinyxml2::XMLElement * entry = feed->FirstChildElement("entry"); entry != NULL; entry = entry->NextSiblingElement("entry")) {
                tinyxml2::XMLElement * system_group = entry->FirstChildElement("gContact:systemGroup");
                if (system_group != NULL && system_group->Attribute("id", "Contacts")) {
                    tinyxml2::XMLElement * id = entry->FirstChildElement("id");
                    if (id) {
                        my_contacts_group_id = id->GetText();
                    }
                }
            }
            if (my_contacts_group_id.empty()) {
                throw std::runtime_error(std::string("Cannot find my contacts group: ").append(c.received_body()));
            }
        }
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
        std::string my_contacts_group_id;
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
