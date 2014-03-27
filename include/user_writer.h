#ifndef GBOOK_ABOOK_USER_WRITER_H
#define GBOOK_ABOOK_USER_WRITER_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "user.h"

namespace gbook {
    namespace abook {
        /**
         * Removes new lines from string. Is group_mode, also replaces ',' with '_'.
         **/
        string sanitize(string input, bool group_mode = false) {
            input.erase(
                remove(begin(input), end(input), '\n'),
                end(input)
            );
            if (group_mode) {
                replace(begin(input), end(input), ',', '_');
            }
            return input;
        }
    }
}


std::ostream& operator<<(std::ostream& os, const user& u) {
    os << "name=" << gbook::abook::sanitize(u.name) << "\n";
    os << "address=" << gbook::abook::sanitize(u.address) << "\n";
    os << "address2=" << gbook::abook::sanitize(u.address2) << "\n";
    os << "city=" << gbook::abook::sanitize(u.city) << "\n";
    os << "state=" << gbook::abook::sanitize(u.state) << "\n";
    os << "zip=" << gbook::abook::sanitize(u.zip) << "\n";
    os << "country=" << gbook::abook::sanitize(u.country) << "\n";
    os << "phone=" << gbook::abook::sanitize(u.phone) << "\n";
    os << "workphone=" << gbook::abook::sanitize(u.workphone) << "\n";
    os << "fax=" << gbook::abook::sanitize(u.fax) << "\n";
    os << "mobile=" << gbook::abook::sanitize(u.mobile) << "\n";
    os << "nick=" << gbook::abook::sanitize(u.nick) << "\n";
    os << "url=" << gbook::abook::sanitize(u.url) << "\n";
    os << "notes=" << gbook::abook::sanitize(u.notes) << "\n";
    os << "anniversary=" << gbook::abook::sanitize(u.anniversary) << "\n";
    os << "custom1=" << gbook::abook::sanitize(u.custom1) << "\n";
    os << "custom2=" << gbook::abook::sanitize(u.custom2) << "\n";
    os << "custom3=" << gbook::abook::sanitize(u.custom3) << "\n";
    os << "custom4=" << gbook::abook::sanitize(u.custom4) << "\n";
    os << "custom5=" << gbook::abook::sanitize(u.custom5) << "\n";

    if (u.emails.size() > 0) {
        os << "email=" << gbook::abook::sanitize(u.emails.at(0), true);
        for (auto it = begin(u.emails) +1; it != end(u.emails); ++it) {
            os << "," << gbook::abook::sanitize(*it, true);
        }
        os << "\n";
    }

    if (u.groups.size() > 0) {
        os << "groups=" << gbook::abook::sanitize(u.groups.at(0), true);
        for (auto it = begin(u.groups) +1; it != end(u.groups); ++it) {
            os << "," << gbook::abook::sanitize(*it, true);
        }
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const vector<user>& users) {
    int user_number = 0;
    for (user u : users) {
        os << "[" << user_number++ << "]\n";
        os << u;
        os << "\n";
    }
    return os;
}

#endif // GBOOK_ABOOK_USER_WRITER_H