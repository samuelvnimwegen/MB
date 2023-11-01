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
    cfg.makeFollow();
    vector<string> followBody = {"<EOS>", "y"};
    EXPECT_TRUE(cfg.getFollow().size() == 1);
    EXPECT_TRUE(cfg.getFollow()[0].getBody().size() == 2);
    EXPECT_TRUE(cfg.getFollow()[0].getBody() == followBody);

    cfg.makeTable();
    vector<string> tableRow = {"`x S y`", "", ""};
    EXPECT_TRUE(cfg.getLlTable().size() == 1);
    EXPECT_TRUE(cfg.getLlTable()[0] == tableRow);
}

TEST_F(LLTest, fulltest2){
    CFG cfg ("input-ll2.json");
    cfg.makeFirst();
    EXPECT_TRUE(cfg.getFirst().size() == cfg.getVariables().size() and cfg.getVariables().size() == 2);
    EXPECT_TRUE(cfg.getFirst()[0].getHead() == cfg.getVariables()[0]);
    EXPECT_TRUE(cfg.getFirst()[1].getHead() == cfg.getVariables()[1]);
    vector<string> firstBody1 = {"c", "|"};
    vector<string> firstBody2 = {"a", "b"};
    EXPECT_TRUE(cfg.getFirst()[0].getBody() == firstBody1);
    EXPECT_TRUE(cfg.getFirst()[1].getBody() == firstBody2);

    cfg.makeFollow();
    EXPECT_TRUE(cfg.getFollow().size() == cfg.getVariables().size() and cfg.getVariables().size() == 2);
    EXPECT_TRUE(cfg.getFollow()[0].getHead() == cfg.getVariables()[0]);
    EXPECT_TRUE(cfg.getFollow()[1].getHead() == cfg.getVariables()[1]);
    vector<string> followBody1 = {"a", "b"};
    vector<string> followBody2 = {"<EOS>", "a", "b"};
    EXPECT_TRUE(cfg.getFollow()[0].getBody() == followBody1);
    EXPECT_TRUE(cfg.getFollow()[1].getBody() == followBody2);

    cfg.makeTable();
    vector<string> tableRow1 = {"", "", "`c S`", "<ERR>"};
    vector<string> tableRow2 = {"`a A b`", "`b A a`", "<ERR>", "<ERR>"};
    EXPECT_TRUE(cfg.getLlTable().size() == 2);
    EXPECT_TRUE(cfg.getLlTable()[0] == tableRow1);
    EXPECT_TRUE(cfg.getLlTable()[1] == tableRow2);
}