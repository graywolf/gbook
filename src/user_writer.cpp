#include "user_writer.h"

#include <string>
#include <algorithm>

using namespace std;
using namespace gbook;

string gbook::sanitize(string input, bool group_mode) {
    input.erase(
        remove(begin(input), end(input), '\n'),
        end(input)
    );
    if (group_mode) {
        for (auto & c : input) {
            if (c == ',') {
                c = '_';
            }
        }
    }
    return input;
}

ostream& operator<<(ostream& os, const user& u) {
    if (!u.name.empty()) {
        os << "name=" << sanitize(u.name) << "\n";
    }
    if (!u.address.empty()) {
        os << "address=" << sanitize(u.address) << "\n";
    }
    if (!u.address2.empty()) {
        os << "address2=" << sanitize(u.address2) << "\n";
    }
    if (!u.city.empty()) {
        os << "city=" << sanitize(u.city) << "\n";
    }
    if (!u.state.empty()) {
        os << "state=" << sanitize(u.state) << "\n";
    }
    if (!u.zip.empty()) {
        os << "zip=" << sanitize(u.zip) << "\n";
    }
    if (!u.country.empty()) {
        os << "country=" << sanitize(u.country) << "\n";
    }
    if (!u.phone.empty()) {
        os << "phone=" << sanitize(u.phone) << "\n";
    }
    if (!u.workphone.empty()) {
        os << "workphone=" << sanitize(u.workphone) << "\n";
    }
    if (!u.fax.empty()) {
        os << "fax=" << sanitize(u.fax) << "\n";
    }
    if (!u.mobile.empty()) {
        os << "mobile=" << sanitize(u.mobile) << "\n";
    }
    if (!u.nick.empty()) {
        os << "nick=" << sanitize(u.nick) << "\n";
    }
    if (!u.url.empty()) {
        os << "url=" << sanitize(u.url) << "\n";
    }
    if (!u.notes.empty()) {
        os << "notes=" << sanitize(u.notes) << "\n";
    }
    if (!u.anniversary.empty()) {
        os << "anniversary=" << sanitize(u.anniversary) << "\n";
    }
    if (!u.custom1.empty()) {
        os << "custom1=" << sanitize(u.custom1) << "\n";
    }
    if (!u.custom2.empty()) {
        os << "custom2=" << sanitize(u.custom2) << "\n";
    }
    if (!u.custom3.empty()) {
        os << "custom3=" << sanitize(u.custom3) << "\n";
    }
    if (!u.custom4.empty()) {
        os << "custom4=" << sanitize(u.custom4) << "\n";
    }
    if (!u.custom5.empty()) {
        os << "custom5=" << sanitize(u.custom5) << "\n";
    }

    if (u.emails.size() > 0) {
        os << "email=" << sanitize(u.emails.at(0), true);
        for (auto it = begin(u.emails) +1; it != end(u.emails); ++it) {
            os << "," << sanitize(*it, true);
        }
        os << "\n";
    }

    if (u.groups.size() > 0) {
        os << "groups=" << sanitize(u.groups.at(0), true);
        for (auto it = begin(u.groups) +1; it != end(u.groups); ++it) {
            os << "," << sanitize(*it, true);
        }
        os << "\n";
    }

    return os;
}

ostream& operator<<(ostream& os, const vector<user>& users) {
    int user_number = 0;
    for (user u : users) {
        os << "[" << user_number++ << "]\n";
        os << u;
        os << "\n";
    }
    return os;
}

ostream& operator<<(ostream& os, const vector<user *>& users) {
    int user_number = 0;
    for (user * u : users) {
        os << "[" << user_number++ << "]\n";
        os << *u;
        os << "\n";
    }
    return os;
}
