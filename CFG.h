//
// Created by Samuel on 01/10/2023.
//

#ifndef MB_CFG_H
#define MB_CFG_H
#include "iostream"
#include "vector"
#include "map"
#include "cassert"
using namespace std;

class CFG {
    vector<string> nonTerminals;
    vector<string> terminals;
    map<string, string> replacementRules;
    string startSymbol;
    CFG* init;
public:
    CFG();

    bool properlyInitialised();

    const vector<string> & getNonTerminals();

    void setNonTerminals(const vector<string> &nonTerminals);

    const vector<string> & getTerminals();

    void setTerminals(const vector<string> &terminals);

    const map<string, string> & getReplacementRules();

    void setReplacementRules(const map<string, string> &replacementRules);

    const string & getStartSymbol();

    void setStartSymbol(const string &startSymbol);
};


#endif //MB_CFG_H
