#include "oauth2.h"
#include "contacts.h"

#include <cstdlib>
#include <fstream>

#include <iostream>

using namespace std;
using namespace gbook;

int main(int argc, char ** argv) {
    string conffile = getenv("HOME");
    conffile.append("/gbook/test/tmp/oauth2.conf");
    oauth2 o2(conffile);
    if (o2.access_token().empty()) {
        user_data ud = o2.request_user_code();
        cout << "User code: " << ud.user_code << endl;
        cout << "Verification url: " << ud.verification_url << endl;
        o2.begin_polling();
        cout << "Ok, we got access token." << endl;
    } else {
        o2.refresh_access_token();
    }

    contacts cs(o2);
    //cs.get_all();

    user u;
    u.name = "Test Tester";
    u.address = "Patkova 3";
    u.address2 = "room 918";
    u.city = "Prague";
    u.state = "Prague province";
    u.zip = "18000";
    u.country = "CR";
    u.phone = "1";
    u.workphone = "2";
    u.fax = "3";
    u.mobile = "4";
    u.nick = "nick";
    u.url = "t.t";
    u.notes = "notest uber notes";
    u.anniversary = "1993-02-24";
    u.custom1 = "c1";
    u.custom2 = "c2";
    u.custom3 = "c3";
    u.custom4 = "c4";
    u.custom5 = "c5";
    u.emails.push_back("tt@example.com");
    u.emails.push_back("tt2@example.com");
    u.groups.push_back("a");
    u.groups.push_back("b");
    cs.add(u);

    u.name = "Test Test2";
    cs.update(u);

    cs.remove(u.get_id("google"));
    return 0;
}
