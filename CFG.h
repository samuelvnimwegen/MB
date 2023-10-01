//
// Created by Samuel on 01/10/2023.
//

#ifndef MB_CFG_H
#define MB_CFG_H
#include "iostream"
#include "vector"
#include "map"
using namespace std;

class CFG {
    vector<string> nonTerminals;
    vector<string> terminals;
    map<string, string> replacementRules;
    string startSymbol;
public:

};


#endif //MB_CFG_H
