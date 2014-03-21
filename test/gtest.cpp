#include "gtest/gtest.h"

using namespace std;

#include "user_reader.cpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
