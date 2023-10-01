//
// Created by Samuel on 01/10/2023.
//
#include "gtest/gtest.h"
#include "CFG.h"

class MBTest: public ::testing::Test{
};

TEST_F(MBTest, test1){
    CFG cfg;
    cfg.setNonTerminals({"BINDIGIT", "S"});
    cfg.setTerminals({"0", "1", "a", "b"});
    cfg.setStartSymbol({"S"});
    vector<pair<string, vector<string>>> replacements;
    vector<string> p1 = {"0"};
    replacements.emplace_back("BINDIGIT", p1);
    vector<string> p2 = {"1"};
    replacements.emplace_back("BINDIGIT", p2);
    vector<string> p3 = {""};
    replacements.emplace_back("S", p3);
    vector<string> p4 = {"a", "S", "b", "BINDIGIT"};
    replacements.emplace_back("S", p4);
    cfg.setReplacementRules(replacements);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}