//
// Created by Samuel on 01/10/2023.
//
#include "gtest/gtest.h"
#include "CFG.h"

class MBTest: public ::testing::Test{
};

TEST_F(MBTest, test0){
    CFG cfg;
    vector<string> nonTerms = {"BINDIGIT", "S"};
    vector<string> terms = {"0", "1", "a", "b"};
    string startSym = "S";

    EXPECT_TRUE(cfg.properlyInitialised());
    auto prod1 = new Production("BINDIGIT", {"0"});
    EXPECT_TRUE(prod1->getHead() == cfg.getProductions()[0]->getHead());
    EXPECT_TRUE(prod1->getBody() == cfg.getProductions()[0]->getBody());

    auto prod2 = new Production("BINDIGIT", {"1"});
    EXPECT_TRUE(prod2->getHead() == cfg.getProductions()[1]->getHead());
    EXPECT_TRUE(prod2->getBody() == cfg.getProductions()[1]->getBody());

    auto prod3 = new Production("S", {});
    EXPECT_TRUE(prod3->getHead() == cfg.getProductions()[2]->getHead());
    EXPECT_TRUE(prod3->getBody() == cfg.getProductions()[2]->getBody());

    auto prod4 = new Production("S", {"a", "S", "b", "BINDIGIT"});
    EXPECT_TRUE(prod4->getHead() == cfg.getProductions()[3]->getHead());
    EXPECT_TRUE(prod4->getBody() == cfg.getProductions()[3]->getBody());

    EXPECT_TRUE(cfg.getTerminals() == terms);
    EXPECT_TRUE(cfg.getNonTerminals() == nonTerms);
    EXPECT_TRUE(cfg.getStartSymbol() == startSym);
}
TEST_F(MBTest, test1){
    CFG cfg;
    vector<string> nonTerms = {"BINDIGIT", "S"};
    vector<string> terms = {"0", "1", "a", "b"};
    string startSym = "S";
    cfg.setNonTerminals(nonTerms);
    cfg.setTerminals(terms);
    cfg.setStartSymbol(startSym);
    vector<Production*> replacements;
    auto prod1 = new Production("BINDIGIT", {"0"});
    replacements.push_back(prod1);
    auto prod2 = new Production("BINDIGIT", {"1"});
    replacements.push_back(prod2);
    auto prod3 = new Production("S", {""});
    replacements.push_back(prod3);
    auto prod4 = new Production("S", {"a", "S", "b", "BINDIGIT"});
    replacements.push_back(prod4);
    cfg.setProductions(replacements);
    EXPECT_TRUE(cfg.properlyInitialised());
    EXPECT_TRUE(cfg.getNonTerminals() == nonTerms);
    EXPECT_TRUE(cfg.getStartSymbol() == startSym);
    EXPECT_TRUE(cfg.getTerminals() == terms);
    EXPECT_TRUE(cfg.getProductions() == replacements);
}

TEST_F(MBTest, test2){
    CFG cfg("test2.json");
    EXPECT_TRUE(cfg.properlyInitialised());
    EXPECT_TRUE(cfg.getStartSymbol() == "S");
    vector<string> nonTerms = {"S", "BINDIGIT"};
    EXPECT_TRUE(cfg.getNonTerminals() == nonTerms);
    vector<string> terms = {"a", "b","0", "1"};
    EXPECT_TRUE(cfg.getTerminals() == terms);
    vector<string> body1 = {"a", "S", "b","BINDIGIT"};
    EXPECT_TRUE(cfg.getProductions()[3]->getHead() == "S");
    EXPECT_TRUE(cfg.getProductions()[3]->getBody() == body1);
    vector<string> body2 = {};
    EXPECT_TRUE(cfg.getProductions()[2]->getHead() == "S");
    EXPECT_TRUE(cfg.getProductions()[2]->getBody() == body2);
    vector<string> body3 = {"0"};
    EXPECT_TRUE(cfg.getProductions()[0]->getHead() == "BINDIGIT");
    EXPECT_TRUE(cfg.getProductions()[0]->getBody() == body3);
    vector<string> body4 = {"1"};
    EXPECT_TRUE(cfg.getProductions()[1]->getHead() == "BINDIGIT");
    EXPECT_TRUE(cfg.getProductions()[1]->getBody() == body4);
}
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}