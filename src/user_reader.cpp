#include "user_reader.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace gbook;

vector<user> gbook::load_users_from_stream(istream & input) {
    string data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return parse_string(data);
}

vector<user> gbook::parse_string(string input) {
    istringstream iss(input);
    string line;

    user current_user;
    bool in_user = false;

    vector<user> users;

    while (getline(iss, line)) {
        if (!line.empty()) {
            if (line == "[format]") {
                in_user = false;
            } else if (line.at(0) == '[') {
                //I don't care about user number, is irrelevant
                in_user = true;
                //if current user is not empty, append it and prepare
                //variable for the new one
                if (!current_user.empty()) {
                    users.push_back(move(current_user));
                    current_user = user();
                }
            } else if (in_user) {
                string key, value;
                //if does not contain =, is not valid so ignore this line
                if (line.find("=") != string::npos) {
                    key = line.substr(0, line.find("="));
                    value = line.substr(line.find("=")+1);

                    if (key == "name") {
                        current_user.name = value;
                    } else if (key == "email") {
                        current_user.emails.clear();
                        while (value.find(",") != string::npos) {
                            current_user.emails.push_back(value.substr(0, value.find(",")));
                            value = value.substr(value.find(",")+1);
                        }
                        current_user.emails.push_back(value);
                    } else if (key == "address") {
                        current_user.address = value;
                    } else if (key == "address2") {
                        current_user.address2 = value;
                    } else if (key == "city") {
                        current_user.city = value;
                    } else if (key == "state") {
                        current_user.state = value;
                    } else if (key == "zip") {
                        current_user.zip = value;
                    } else if (key == "country") {
                        current_user.country = value;
                    } else if (key == "phone") {
                        current_user.phone = value;
                    } else if (key == "workphone") {
                        current_user.workphone = value;
                    } else if (key == "fax") {
                        current_user.fax = value;
                    } else if (key == "mobile") {
                        current_user.mobile = value;
                    } else if (key == "nick") {
                        current_user.nick = value;
                    } else if (key == "url") {
                        current_user.url = value;
                    } else if (key == "notes") {
                        current_user.notes = value;
                    } else if (key == "anniversary") {
                        current_user.anniversary = value;
                    } else if (key == "groups") {
                        current_user.groups.clear();
                        while (value.find(",") != string::npos) {
                            current_user.groups.push_back(value.substr(0, value.find(",")));
                            value = value.substr(value.find(",")+1);
                        }
                        current_user.groups.push_back(value);
                    } else if (key == "custom1") {
                        current_user.custom1 = value;
                    } else if (key == "custom2") {
                        current_user.custom2 = value;
                    } else if (key == "custom3") {
                        current_user.custom3 = value;
                    } else if (key == "custom4") {
                        current_user.custom4 = value;
                    } else if (key == "custom5") {
                        current_user.custom5 = value;
                        current_user.set_id("abook", value);
                    } else {
                        //uknown key, let's ignore this line
                    }
                }
            }
        }
    }
    if (!current_user.empty()) {
        users.push_back(move(current_user));
    }
    return users;
}
