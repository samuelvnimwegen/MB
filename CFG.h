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
    vector<Production*> productions;
    string startSymbol;
    CFG* init;
    string filename;
public:
    CFG();

    explicit CFG(const string &name);

    bool properlyInitialised();

    const vector<string> & getNonTerminals();

    void setNonTerminals(const vector<string> &nonTerminals);

    const vector<string> & getTerminals();

    void setTerminals(const vector<string> &terminals);

    const vector<Production*> & getProductions();

    void setProductions(const vector<Production*> &rules);

    const string & getStartSymbol();

    void setStartSymbol(const string &startSymbol);

    void addTerminal(const string &term);

    void addNonTerminal(const string &term);

    void addProduction(Production* prod);

    vector<string> print();
};

vector<Production*> sortProds(const vector<Production*>& prods);

#endif //MB_CFG_H
