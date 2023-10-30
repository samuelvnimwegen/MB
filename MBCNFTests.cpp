//
// Created by Samuel on 30/10/2023.
//
#include "CFG.h"
#include "gtest/gtest.h"
class MBTest: public ::testing::Test{
};

TEST_F(MBTest, test0){
CFG cfg;
vector<string> nonTerms = {"BINDIGIT", "S"};
vector<string> terms = {"0", "1", "a", "b"};
string startSym = "S";

EXPECT_TRUE(cfg.properlyInitialised());
auto prod1 = new Production("BINDIGIT", {"0"});
EXPECT_TRUE(prod1->getHead() == cfg.getProductions()[0].getHead());
EXPECT_TRUE(prod1->getBody() == cfg.getProductions()[0].getBody());

auto prod2 = new Production("BINDIGIT", {"1"});
EXPECT_TRUE(prod2->getHead() == cfg.getProductions()[1].getHead());
EXPECT_TRUE(prod2->getBody() == cfg.getProductions()[1].getBody());

auto prod3 = new Production("S", {});
EXPECT_TRUE(prod3->getHead() == cfg.getProductions()[2].getHead());
EXPECT_TRUE(prod3->getBody() == cfg.getProductions()[2].getBody());

auto prod4 = new Production("S", {"a", "S", "b", "BINDIGIT"});
EXPECT_TRUE(prod4->getHead() == cfg.getProductions()[3].getHead());
EXPECT_TRUE(prod4->getBody() == cfg.getProductions()[3].getBody());

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
vector<Production> replacements;
auto prod1 =  Production("BINDIGIT", {"0"});
replacements.push_back(prod1);
auto prod2 =  Production("BINDIGIT", {"1"});
replacements.push_back(prod2);
auto prod3 =  Production("S", {""});
replacements.push_back(prod3);
auto prod4 =  Production("S", {"a", "S", "b", "BINDIGIT"});
replacements.push_back(prod4);
cfg.setProductions(replacements);
EXPECT_TRUE(cfg.properlyInitialised());
EXPECT_TRUE(cfg.getNonTerminals() == nonTerms);
EXPECT_TRUE(cfg.getStartSymbol() == startSym);
EXPECT_TRUE(cfg.getTerminals() == terms);
for (int i = 0; i < replacements.size(); ++i){
EXPECT_TRUE(cfg.getProductions()[i].getHead() == replacements[i].getHead() and cfg.getProductions()[i].getBody() == replacements[i].getBody());
}
}

TEST_F(MBTest, test2){
CFG cfg("test2.json");
EXPECT_TRUE(cfg.properlyInitialised());
EXPECT_TRUE(cfg.getStartSymbol() == "S");
vector<string> nonTerms = {"BINDIGIT", "S"};
EXPECT_TRUE(cfg.getNonTerminals() == nonTerms);
vector<string> terms = {"0", "1", "a", "b"};
EXPECT_TRUE(cfg.getTerminals() == terms);
vector<string> body1 = {"a", "S", "b","BINDIGIT"};
EXPECT_TRUE(cfg.getProductions()[3].getHead() == "S");
EXPECT_TRUE(cfg.getProductions()[3].getBody() == body1);
vector<string> body2 = {};
EXPECT_TRUE(cfg.getProductions()[2].getHead() == "S");
EXPECT_TRUE(cfg.getProductions()[2].getBody() == body2);
vector<string> body3 = {"0"};
EXPECT_TRUE(cfg.getProductions()[0].getHead() == "BINDIGIT");
EXPECT_TRUE(cfg.getProductions()[0].getBody() == body3);
vector<string> body4 = {"1"};
EXPECT_TRUE(cfg.getProductions()[1].getHead() == "BINDIGIT");
EXPECT_TRUE(cfg.getProductions()[1].getBody() == body4);
}

TEST_F(MBTest, isNullableTest){
CFG cfg("input-cnf1.json");
EXPECT_TRUE(cfg.isNullable({"S"}));
EXPECT_TRUE(cfg.isNullable({"A"}));
EXPECT_TRUE(cfg.isNullable({"B"}));
EXPECT_TRUE(cfg.isNullable({"C"}));
EXPECT_TRUE(cfg.isNullable({"D"}));
EXPECT_FALSE(cfg.isNullable({"E"}));
}

TEST_F(MBTest, fulltest1){
// Eerst de input controleren:
auto cfg = CFG("input-cnf1.json");
EXPECT_TRUE(cfg.properlyInitialised());
vector<string> variables = {"A", "B", "C", "D", "E", "S"};
EXPECT_TRUE(cfg.getNonTerminals() == variables);
vector<string> terminals = {"a", "b"};
vector<string> productionStrings = {
        "A -> `C`",
        "A -> `a`",
        "B -> `C`",
        "B -> `b`",
        "C -> `C D E`",
        "C -> ``",
        "D -> `A`",
        "D -> `B`",
        "D -> `a b`",
        "S -> ``",
        "S -> `a A a`",
        "S -> `b B b`"
};
for (int i = 0; i < cfg.getProductions().size(); ++i){
EXPECT_TRUE(productionStrings[i] == cfg.getProductions()[i].getSortString());
}
EXPECT_TRUE(cfg.getStartSymbol() == "S");

// Nu de epsilon producties verwijderen.
cfg.removeNullable();
vector<string> nullables = {"A", "B", "C", "D", "S"};
productionStrings = {
        "A -> `C`",
        "A -> `a`",
        "B -> `C`",
        "B -> `b`",
        "C -> `C D E`",
        "C -> `C E`",
        "C -> `D E`",
        "C -> `E`",
        "D -> `A`",
        "D -> `B`",
        "D -> `a b`",
        "S -> `a A a`",
        "S -> `a a`",
        "S -> `b B b`",
        "S -> `b b`"
};
for (int i = 0; i < cfg.getProductions().size(); ++i){
EXPECT_TRUE(cfg.getProductions()[i].getSortString() == productionStrings[i]);
}
EXPECT_TRUE(cfg.getStartSymbol() == "S");

// Unit pairs verwijderen
cfg.removeUnitPairs();
vector<pair<string, string>> unitPairs = {
        make_pair("A", "A"), make_pair("A", "C"), make_pair("A", "E"), make_pair("B", "B"), make_pair("B", "C"),
        make_pair("B", "E"), make_pair("C", "C"), make_pair("C", "E"), make_pair("D", "A"),
        make_pair("D", "B"), make_pair("D", "C"), make_pair("D", "D"), make_pair("D", "E"), make_pair("E", "E"),make_pair("S", "S")
};
EXPECT_TRUE(cfg.getNonTerminals() == variables);
EXPECT_TRUE(cfg.getTerminals() == terminals);
productionStrings = {
        "A -> `C D E`",
        "A -> `C E`",
        "A -> `D E`",
        "A -> `a`",
        "B -> `C D E`",
        "B -> `C E`",
        "B -> `D E`",
        "B -> `b`",
        "C -> `C D E`",
        "C -> `C E`",
        "C -> `D E`",
        "D -> `C D E`",
        "D -> `C E`",
        "D -> `D E`",
        "D -> `a b`",
        "D -> `a`",
        "D -> `b`",
        "S -> `a A a`",
        "S -> `a a`",
        "S -> `b B b`",
        "S -> `b b`",
};
for (int i = 0; i < cfg.getProductions().size(); ++i){
EXPECT_TRUE(cfg.getProductions()[i].getSortString() == productionStrings[i]);
}
EXPECT_TRUE(cfg.getStartSymbol() == "S");
}