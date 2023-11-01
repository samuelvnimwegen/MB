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
    vector<string> variables;
    vector<string> terminals;
    vector<Production> productions;
    string startSymbol;
    CFG* init;
    string filename;
    vector<Production> first;
    vector<Production> follow;
    vector<vector<string>> LLTable;
    vector<pair<string, Production>> firstPairs;
public:

    bool properlyInitialised();

    CFG();

    explicit CFG(const string &name);

    CFG toCNF();

    void ll();

    void removeNullable();

    void removeUnitPairs();

    void removeUseless();

    void replaceBadBodies();

    void breakBodies();

    void makeFirst();

    void makeFollow();

    void makeTable();

    void printTable();

    bool isNullable(vector<string> inputString);

    vector<string> print();

    /////////////////////////
    // Getters en setters: //
    /////////////////////////

    void addTableRow(const vector<string> &row);

    const vector<string> & getVariables();

    void setVariables(const vector<string> &nonTerminals);

    const vector<string> & getTerminals();

    void setTerminals(const vector<string> &terminals);

    const vector<Production> & getProductions();

    void setProductions(const vector<Production> &rules);

    const string & getStartSymbol();

    void setStartSymbol(const string &startSymbol);

    void addTerminal(const string &term);

    void addVariables(const string &term);

    void addProduction(const Production& prod);

    [[nodiscard]] const vector<Production> &getFirst() const;

    void setFirst(const vector<Production> &fr);

    [[nodiscard]] const vector<Production> &getFollow() const;

    void setFollow(const vector<Production> &fl);

    [[nodiscard]] const vector<vector<string>> &getLlTable() const;

    void setLlTable(const vector<vector<string>> &llTable);

    [[nodiscard]] const vector<pair<string, Production>> &getFirstPairs() const;

    void setFirstPairs(const vector<pair<string, Production>> &fp);

    void addFirstPair(const pair<string, Production>& pair);

};

vector<Production> sortProds(const vector<Production> &prods);

vector<Production> removeDupes(const vector<Production> &prods);

void makeNullableSubsets(vector<vector<string>> &subsets, const vector<string> &nullables, const vector<string> &current);

void printVector(const vector<string> &items);

string breakBodyName(const string &head, const vector<string> &vars);

Production mergeProductions(const vector<Production> &productions);

string generateSpace(const int &size);


#endif //MB_CFG_H
