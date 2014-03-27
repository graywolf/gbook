#include "user_writer.h"

#include <sstream>

using namespace std;
using namespace gbook;

TEST (GbookAbookUserWriter, writeOneFullUser) {
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

    vector<user> us;
    us.push_back(u);
    stringstream ss;
    ss << us;
    ASSERT_EQ("[0]\n\
name=Test Tester\n\
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
custom1=c1\n\
custom2=c2\n\
custom3=c3\n\
custom4=c4\n\
custom5=c5\n\
email=tt@example.com,tt2@example.com\n\
groups=a,b\n", ss.str());
    us.push_back(u);
    ss.str("");
    ss << us;
    ASSERT_EQ("[0]\n\
name=Test Tester\n\
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
custom1=c1\n\
custom2=c2\n\
custom3=c3\n\
custom4=c4\n\
custom5=c5\n\
email=tt@example.com,tt2@example.com\n\
groups=a,b\n\
[1]\n\
name=Test Tester\n\
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
custom1=c1\n\
custom2=c2\n\
custom3=c3\n\
custom4=c4\n\
custom5=c5\n\
email=tt@example.com,tt2@example.com\n\
groups=a,b\n", ss.str());
}
