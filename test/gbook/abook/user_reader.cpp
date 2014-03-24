#include "include/user_reader.h"

#include <sstream>

using namespace std;
using namespace gbook::abook;

TEST (GbookAbookUserReader, loadOneFullUserFromStream) {
    stringstream ss;
    ss.str("# abook addressbook file\n\
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
custom5=c5");
    vector<user> users = load_users_from_stream(ss);
    ASSERT_EQ(1, users.size());
    ASSERT_EQ("Test Tester", users[0].name);
    ASSERT_EQ("Patkova 3", users[0].address);
    ASSERT_EQ("room 918", users[0].address2);
    ASSERT_EQ("Prague", users[0].city);
    ASSERT_EQ("Prague province", users[0].state);
    ASSERT_EQ("18000", users[0].zip);
    ASSERT_EQ("CR", users[0].country);
    ASSERT_EQ("1", users[0].phone);
    ASSERT_EQ("2", users[0].workphone);
    ASSERT_EQ("3", users[0].fax);
    ASSERT_EQ("4", users[0].mobile);
    ASSERT_EQ("nick", users[0].nick);
    ASSERT_EQ("t.t", users[0].url);
    ASSERT_EQ("notest uber notes", users[0].notes);
    ASSERT_EQ("1993-02-24", users[0].anniversary);
    ASSERT_EQ("c1", users[0].custom1);
    ASSERT_EQ("c2", users[0].custom2);
    ASSERT_EQ("c3", users[0].custom3);
    ASSERT_EQ("c4", users[0].custom4);
    ASSERT_EQ("c5", users[0].custom5);

    ASSERT_EQ(2, users[0].emails.size());
    ASSERT_EQ("tt@example.com", users[0].emails[0]);
    ASSERT_EQ("tt2@example.com", users[0].emails[1]);
    ASSERT_EQ(2, users[0].groups.size());
    ASSERT_EQ("a", users[0].groups[0]);
    ASSERT_EQ("b", users[0].groups[1]);
}

TEST (GbookAbookUserReader, loadMoreUsersFromStream) {
    stringstream ss;
    ss.str("# abook addressbook file\n\
\n\
[format]\n\
program=abook\n\
version=0.6.0pre2\n\
\n\
\n\
[0]\n\
name=t0\n\
[1]\n\
name=t1\n\
[2]\n\
name=t2");
    vector<user> users = load_users_from_stream(ss);
    ASSERT_EQ(3, users.size());
    ASSERT_EQ("t0", users[0].name);
    ASSERT_EQ("t1", users[1].name);
    ASSERT_EQ("t2", users[2].name);
}
