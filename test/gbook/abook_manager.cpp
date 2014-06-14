#include <sstream>

using namespace std;
using namespace gbook;

#include "abook_manager.h"

user u1() {
    user u;
    u.name = "u 1";
    u.custom5 = "1";
    return u;
}

user u2() {
    user u;
    u.name = "u 2";
    u.custom5 = "2";
    return u;
}

user u3() {
    user u;
    u.name = "u 3";
    u.custom5 = "3";
    return u;
}

user u4() {
    user u;
    u.name = "u 4";
    u.custom5 = "4";
    return u;
}

user u4_() {
    user u;
    u.name = "u 5";
    u.custom5 = "4";
    return u;
}

user ue() {
    user u;
    u.name = "u e";
    return u;
}

user_list l12    = { u1(), u2() };
user_list l1e    = { u1(), ue() };
user_list l1     = { u1() };
user_list le     = { ue() };
user_list l4     = { u4() };
user_list l4_    = { u4_() };
user_list l12345 = { u1(), u2(), u3(), u4(), u4_()};

TEST(GbookAbookManager, noChanges) {
    storage_changes c;
    abook_manager m;
    stringstream ss;
    ss << l12;
    m.load(ss);
    m.fill_storage_changes(c, l12);
    ASSERT_EQ((size_t) 0, c.new_users.size());
    ASSERT_EQ((size_t) 0, c.modified_users.size());
    ASSERT_EQ((size_t) 0, c.deleted_users.size());
}

TEST(GbookAbookManager, oneNew) {
    storage_changes c;
    abook_manager m;
    stringstream ss;
    ss << l1e;
    m.load(ss);
    m.fill_storage_changes(c, l1);
    ASSERT_EQ((size_t) 1, c.new_users.size());
    ASSERT_EQ((size_t) 0, c.modified_users.size());
    ASSERT_EQ((size_t) 0, c.deleted_users.size());
}

TEST(GbookAbookManager, oneModified) {
    storage_changes c;
    abook_manager m;
    stringstream ss;
    ss << l4_;
    m.load(ss);
    m.fill_storage_changes(c, l4);
    ASSERT_EQ((size_t) 0, c.new_users.size());
    ASSERT_EQ((size_t) 1, c.modified_users.size());
    ASSERT_EQ((size_t) 0, c.deleted_users.size());
}

TEST(GbookAbookManager, oneDeleted) {
    storage_changes c;
    abook_manager m;
    stringstream ss;
    ss << l1;
    m.load(ss);
    m.fill_storage_changes(c, l12);
    ASSERT_EQ((size_t) 0, c.new_users.size());
    ASSERT_EQ((size_t) 0, c.modified_users.size());
    ASSERT_EQ((size_t) 1, c.deleted_users.size());
}
