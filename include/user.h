#ifndef GBOOK_ABOOK_USER_H
#define GBOOK_ABOOK_USER_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

namespace gbook {
    /**
        * Class which hold user data readed from abook data file.
        **/
    class user {
    public:
        /**
            * Name
            **/
        std::string name;
        /**
            * Emails
            **/
        std::vector<std::string> emails;
        /**
            * Address line 1
            **/
        std::string address;
        /**
            * Address line 2
            **/
        std::string address2;
        /**
            * City
            **/
        std::string city;
        /**
            * State
            **/
        std::string state;
        /**
            * ZIP code
            **/
        std::string zip;
        /**
            * Country
            **/
        std::string country;
        /**
            * Phone
            **/
        std::string phone;
        /**
            * Workphone
            **/
        std::string workphone;
        /**
            * Fax
            **/
        std::string fax;
        /**
            * Mobile
            **/
        std::string mobile;
        /**
            * Nick
            **/
        std::string nick;
        /**
            * Url
            **/
        std::string url;
        /**
            * Notes
            **/
        std::string notes;
        /**
            * Anniversary in format YYYY-MM-DD
            **/
        std::string anniversary;
        /**
            * Groups into which user belongs separated by comma
            **/
        std::vector<std::string> groups;
        /**
            * Slot for custom information 1
            **/
        std::string custom1;
        /**
            * Slot for custom information 2
            **/
        std::string custom2;
        /**
            * Slot for custom information 3
            **/
        std::string custom3;
        /**
            * Slot for custom information 4
            **/
        std::string custom4;
        /**
            * Slot for custom information 5
            **/
        std::string custom5;

        /**
            * Checks if user is empty.
            **/
        bool empty() {
            return name.empty()
                    && emails.empty()
                    && address.empty()
                    && address2.empty()
                    && city.empty()
                    && state.empty()
                    && zip.empty()
                    && country.empty()
                    && phone.empty()
                    && workphone.empty()
                    && fax.empty()
                    && mobile.empty()
                    && nick.empty()
                    && url.empty()
                    && notes.empty()
                    && anniversary.empty()
                    && groups.empty()
                    && custom1.empty()
                    && custom2.empty()
                    && custom3.empty()
                    && custom4.empty()
                    && custom5.empty();
        }

        bool operator==(const user & other) {
            return     name == other.name
                    && emails == other.emails
                    && address == other.address
                    && address2 == other.address2
                    && city == other.city
                    && state == other.state
                    && zip == other.zip
                    && country == other.country
                    && phone == other.phone
                    && workphone == other.workphone
                    && fax == other.fax
                    && mobile == other.mobile
                    && nick == other.nick
                    && url == other.url
                    && notes == other.notes
                    && anniversary == other.anniversary
                    && groups == other.groups
                    && custom1 == other.custom1
                    && custom2 == other.custom2
                    && custom3 == other.custom3
                    && custom4 == other.custom4
                    && custom5 == other.custom5;
        }

        bool operator!=(const user & other) {
            return ! (*this == other);
        }

        std::string get_id(std::string key) {
            auto it = ids_.find(key);
            if (it != ids_.end()) {
                return ids_.find(key)->second;
            } else {
                throw std::invalid_argument("No such id.");
            }
        }
        void set_id(std::string key, std::string id) {
            ids_[key] = id;
        }
        bool has_address() {
            return ! (address.empty()
                    &&address2.empty()
                    &&city.empty()
                    &&zip.empty()
                    &&state.empty()
                    &&country.empty()
            );
        }
    private:
        std::map<std::string, std::string> ids_;
    };
}

#endif // GBOOK_ABOOK_USER_H
