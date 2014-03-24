#include "gtest/gtest.h"

using namespace std;

#include "gbook/abook/user_reader.cpp"
#include "gbook/abook/user_writer.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
