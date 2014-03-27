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
    cs.get_all();
    return 0;
}
