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
    os << "name=" << gbook::sanitize(u.name) << "\n";
    os << "address=" << gbook::sanitize(u.address) << "\n";
    os << "address2=" << gbook::sanitize(u.address2) << "\n";
    os << "city=" << gbook::sanitize(u.city) << "\n";
    os << "state=" << gbook::sanitize(u.state) << "\n";
    os << "zip=" << gbook::sanitize(u.zip) << "\n";
    os << "country=" << gbook::sanitize(u.country) << "\n";
    os << "phone=" << gbook::sanitize(u.phone) << "\n";
    os << "workphone=" << gbook::sanitize(u.workphone) << "\n";
    os << "fax=" << gbook::sanitize(u.fax) << "\n";
    os << "mobile=" << gbook::sanitize(u.mobile) << "\n";
    os << "nick=" << gbook::sanitize(u.nick) << "\n";
    os << "url=" << gbook::sanitize(u.url) << "\n";
    os << "notes=" << gbook::sanitize(u.notes) << "\n";
    os << "anniversary=" << gbook::sanitize(u.anniversary) << "\n";
    os << "custom1=" << gbook::sanitize(u.custom1) << "\n";
    os << "custom2=" << gbook::sanitize(u.custom2) << "\n";
    os << "custom3=" << gbook::sanitize(u.custom3) << "\n";
    os << "custom4=" << gbook::sanitize(u.custom4) << "\n";
    os << "custom5=" << gbook::sanitize(u.custom5) << "\n";

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
