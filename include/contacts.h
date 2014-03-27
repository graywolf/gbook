#ifndef libgcon_CONTACTS_H
#define libgcon_CONTACTS_H

#include "curl.h"
#include "oauth2.h"

#include "../lib/tinyxml2/tinyxml2.h"

#include <iostream>

namespace libgcon {
    class contacts {
    public:
        contacts(oauth2 o2) : oauth2_(o2) {}
        void get_all() {
            curl c;
            prepare_curl(c);
            c.url("https://www.google.com/m8/feeds/contacts/default/full");
            c.method(method::GET);
            c.execute();

            tinyxml2::XMLDocument d;
            d.Parse(c.received_body().c_str());
        }
    private:
        void prepare_curl(curl & c) {
            c.header("GData-Version", "3.0");
            c.header("Authorization", std::string("Bearer ").append(oauth2_.access_token()));
        }
        oauth2 & oauth2_;
    };
}

#endif // libgcon_CONTACTS_H
