//
// Created by Samuel on 01/10/2023.
//
#include "gtest/gtest.h"
using namespace std;

class MBTest: public ::testing::Test{
};

TEST_F(MBTest, test){
    EXPECT_TRUE(true);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}