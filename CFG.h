//
// Created by Samuel on 01/10/2023.
//

#ifndef MB_CFG_H
#define MB_CFG_H
#include "iostream"
#include "vector"
#include "map"
#include "cassert"
#include "algorithm"
#include "json.hpp"
#include "fstream"
#include "Production.h"
using namespace std;
using json = nlohmann::json;

class CFG {
    vector<string> nonTerminals;
    vector<string> terminals;
    vector<Production> productions;
    string startSymbol;
    CFG* init;
    string filename;
public:

    bool properlyInitialised();

    CFG();

    explicit CFG(const string &name);

    CFG toCNF();

    void removeNullable();

    void removeUnitPairs();

    bool isNullable(vector<string> inputString);

    vector<string> print();

    /////////////////////////
    // Getters en setters: //
    /////////////////////////

    const vector<string> & getNonTerminals();

    void setNonTerminals(const vector<string> &nonTerminals);

    const vector<string> & getTerminals();

    void setTerminals(const vector<string> &terminals);

    const vector<Production> & getProductions();

    void setProductions(const vector<Production> &rules);

    const string & getStartSymbol();

    void setStartSymbol(const string &startSymbol);

    void addTerminal(const string &term);

    void addNonTerminal(const string &term);

    void addProduction(const Production& prod);
};

vector<Production> sortProds(const vector<Production> &prods);

vector<Production> removeDupes(const vector<Production> &prods);

void makeNullableSubsets(vector<vector<string>> &subsets, const vector<string> &nullables, const vector<string> &current);

#endif //MB_CFG_H
