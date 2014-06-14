#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std;

#include "gbook/abook/user_reader.cpp"
#include "gbook/abook/user_writer.cpp"

#include "gbook/abook_manager.cpp"
#include "gbook/merger.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
