#ifndef GBOOK_ABOOK_USER_H
#define GBOOK_ABOOK_USER_H

#include <string>
#include <vector>

namespace gbook {
    namespace abook {
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
        private:
        };
    }
}

#endif // GBOOK_ABOOK_USER_H