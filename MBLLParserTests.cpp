//
// Created by Samuel on 30/10/2023.
//
#include "gtest/gtest.h"
#include "CFG.h"
class LLTest: public ::testing::Test{
};

TEST_F(LLTest, fulltest1){
    CFG cfg("input-ll1.json");
    cfg.makeFirst();
    EXPECT_TRUE(cfg.getFirst().size() == cfg.getVariables().size() == 1);
    EXPECT_TRUE(cfg.getFirst()[0].getHead() == "S");
    vector<string> body = {"x", "|"};
    EXPECT_TRUE(cfg.getFirst()[0].getBody().size() == 2);
    EXPECT_TRUE(cfg.getFirst()[0].getBody() == body);
}