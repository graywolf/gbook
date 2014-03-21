#include "include/user_reader.h"

#include <fstream>

using namespace std;
using namespace gbook::abook;

TEST (GbookAbookUserReader, loadUsersFromFile) {
    ofstream outf("/tmp/test_file");
    outf << "# abook addressbook file\n\
\n\
[format]\n\
program=abook\n\
version=0.6.0pre2\n\
\n\
\n\
[0]\n\
name=Test Tester\n\
email=tt@example.com,tt2@example.com\n\
address=Patkova 3\n\
address2=room 918\n\
city=Prague\n\
state=Prague province\n\
zip=18000\n\
country=CR\n\
phone=1\n\
workphone=2\n\
fax=3\n\
mobile=4\n\
nick=nick\n\
url=t.t\n\
notes=notest uber notes\n\
anniversary=1993-02-24\n\
groups=a,b\n\
custom1=c1\n\
custom2=c2\n\
custom3=c3\n\
custom4=c4\n\
custom5=c5";
    outf.close();
    ifstream inf("/tmp/test_file");
    vector<user> users = load_users_from_stream(inf);
    ASSERT_EQ(1, users.size());
    ASSERT_EQ("Test Tester", users[0].name);
}
