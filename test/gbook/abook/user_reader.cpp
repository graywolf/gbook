#include "user_reader.h"

#include <sstream>

using namespace std;
using namespace gbook;

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
    auto users = load_users_from_stream(ss);
    ASSERT_EQ((size_t) 1, users.size());
    ASSERT_EQ("Test Tester", begin(users)->name);
    ASSERT_EQ("Patkova 3", begin(users)->address);
    ASSERT_EQ("room 918", begin(users)->address2);
    ASSERT_EQ("Prague", begin(users)->city);
    ASSERT_EQ("Prague province", begin(users)->state);
    ASSERT_EQ("18000", begin(users)->zip);
    ASSERT_EQ("CR", begin(users)->country);
    ASSERT_EQ("1", begin(users)->phone);
    ASSERT_EQ("2", begin(users)->workphone);
    ASSERT_EQ("3", begin(users)->fax);
    ASSERT_EQ("4", begin(users)->mobile);
    ASSERT_EQ("nick", begin(users)->nick);
    ASSERT_EQ("t.t", begin(users)->url);
    ASSERT_EQ("notest uber notes", begin(users)->notes);
    ASSERT_EQ("1993-02-24", begin(users)->anniversary);
    ASSERT_EQ("c1", begin(users)->custom1);
    ASSERT_EQ("c2", begin(users)->custom2);
    ASSERT_EQ("c3", begin(users)->custom3);
    ASSERT_EQ("c4", begin(users)->custom4);
    ASSERT_EQ("c5", begin(users)->custom5);

    ASSERT_EQ((size_t) 2, begin(users)->emails.size());
    ASSERT_EQ("tt@example.com", begin(users)->emails[0]);
    ASSERT_EQ("tt2@example.com", begin(users)->emails[1]);
    ASSERT_EQ((size_t) 2, begin(users)->groups.size());
    ASSERT_EQ("a", begin(users)->groups[0]);
    ASSERT_EQ("b", begin(users)->groups[1]);
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
    auto users = load_users_from_stream(ss);
    ASSERT_EQ((size_t) 3, users.size());
    auto it = begin(users);
    ASSERT_EQ("t0", (it++)->name);
    ASSERT_EQ("t1", (it++)->name);
    ASSERT_EQ("t2", it->name);
}
