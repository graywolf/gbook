#include "user_writer.h"

std::string gbook::sanitize(std::string input, bool group_mode) {
    input.erase(
        remove(begin(input), end(input), '\n'),
                end(input)
    );
    if (group_mode) {
        replace(begin(input), end(input), ',', '_');
    }
    return input;
}

std::ostream& operator<<(std::ostream& os, const gbook::user& u) {
    if (!u.name.empty()) {
        os << "name=" << gbook::sanitize(u.name) << "\n";
    }
    if (!u.address.empty()) {
        os << "address=" << gbook::sanitize(u.address) << "\n";
    }
    if (!u.address2.empty()) {
        os << "address2=" << gbook::sanitize(u.address2) << "\n";
    }
    if (!u.city.empty()) {
        os << "city=" << gbook::sanitize(u.city) << "\n";
    }
    if (!u.state.empty()) {
        os << "state=" << gbook::sanitize(u.state) << "\n";
    }
    if (!u.zip.empty()) {
        os << "zip=" << gbook::sanitize(u.zip) << "\n";
    }
    if (!u.country.empty()) {
        os << "country=" << gbook::sanitize(u.country) << "\n";
    }
    if (!u.phone.empty()) {
        os << "phone=" << gbook::sanitize(u.phone) << "\n";
    }
    if (!u.workphone.empty()) {
        os << "workphone=" << gbook::sanitize(u.workphone) << "\n";
    }
    if (!u.fax.empty()) {
        os << "fax=" << gbook::sanitize(u.fax) << "\n";
    }
    if (!u.mobile.empty()) {
        os << "mobile=" << gbook::sanitize(u.mobile) << "\n";
    }
    if (!u.nick.empty()) {
        os << "nick=" << gbook::sanitize(u.nick) << "\n";
    }
    if (!u.url.empty()) {
        os << "url=" << gbook::sanitize(u.url) << "\n";
    }
    if (!u.notes.empty()) {
        os << "notes=" << gbook::sanitize(u.notes) << "\n";
    }
    if (!u.anniversary.empty()) {
        os << "anniversary=" << gbook::sanitize(u.anniversary) << "\n";
    }
    if (!u.custom1.empty()) {
        os << "custom1=" << gbook::sanitize(u.custom1) << "\n";
    }
    if (!u.custom2.empty()) {
        os << "custom2=" << gbook::sanitize(u.custom2) << "\n";
    }
    if (!u.custom3.empty()) {
        os << "custom3=" << gbook::sanitize(u.custom3) << "\n";
    }
    if (!u.custom4.empty()) {
        os << "custom4=" << gbook::sanitize(u.custom4) << "\n";
    }
    if (!u.custom5.empty()) {
        os << "custom5=" << gbook::sanitize(u.custom5) << "\n";
    }

    if (u.emails.size() > 0) {
        os << "email=" << gbook::sanitize(u.emails.at(0), true);
        for (auto it = begin(u.emails) +1; it != end(u.emails); ++it) {
            os << "," << gbook::sanitize(*it, true);
        }
        os << "\n";
    }

    if (u.groups.size() > 0) {
        os << "groups=" << gbook::sanitize(u.groups.at(0), true);
        for (auto it = begin(u.groups) +1; it != end(u.groups); ++it) {
            os << "," << gbook::sanitize(*it, true);
        }
        os << "\n";
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<gbook::user>& users) {
    int user_number = 0;
    for (gbook::user u : users) {
        os << "[" << user_number++ << "]\n";
        os << u;
        os << "\n";
    }
    return os;
}
