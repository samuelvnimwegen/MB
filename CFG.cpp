//
// Created by Samuel on 01/10/2023.
//

#include "CFG.h"

CFG::CFG() {
    init = this;

}


const vector<string> & CFG::getNonTerminals() {
    assert(this->properlyInitialised());
    auto result = nonTerminals;
    assert(!result.empty());
    return nonTerminals;
}

void CFG::setNonTerminals(const vector<string> &nt) {
    assert(this->properlyInitialised());
    assert(!nt.empty());
    CFG::nonTerminals = nt;
    assert(this->getNonTerminals() == nt);
}

const vector<string> & CFG::getTerminals() {
    assert(this->properlyInitialised());
    assert(!terminals.empty());
    return terminals;
}

void CFG::setTerminals(const vector<string> &term) {
    assert(this->properlyInitialised());
    assert(!term.empty());
    CFG::terminals = term;
    assert(this->getTerminals() == term);
}

const vector<pair<string, vector<string>>> & CFG::getReplacementRules() {
    assert(this->properlyInitialised());
    return replacementRules;
}

void CFG::setReplacementRules(const vector<pair<string, vector<string>>> &rules) {
    assert(this->properlyInitialised());
    assert(!rules.empty());
    CFG::replacementRules = rules;
    assert(this->getReplacementRules() == rules);
}

const string & CFG::getStartSymbol() {
    assert(this->properlyInitialised());
    assert(!startSymbol.empty());
    return startSymbol;
}

void CFG::setStartSymbol(const string &startSym) {
    assert(this->properlyInitialised());
    assert(!startSym.empty());
    assert(count(nonTerminals.begin(), nonTerminals.end(),startSym));
    CFG::startSymbol = startSym;
    assert(this->getStartSymbol() == startSym);

}

bool CFG::properlyInitialised() {
    return init == this;
}

vector<string> CFG::print() {
    vector<string> output;

    // Non-terminals
    string nonTerms = "V = {";
    for (const auto& sym: this->getNonTerminals()){
        nonTerms += sym + ", ";
    }
    nonTerms.pop_back(); nonTerms.pop_back();
    nonTerms += "}";
    output.push_back(nonTerms);
    cout << nonTerms << endl;

    // Terminals
    string terms = "T = {";
    for (const auto& sym: this->getTerminals()){
        terms += sym + ", ";
    }
    terms.pop_back(); terms.pop_back();
    terms += "}";
    output.push_back(terms);
    cout << terms << endl;

    // Transitions
    string transBegin = "P = {";
    output.push_back(transBegin);
    cout << transBegin << endl;
    for (const auto& rep: this->getReplacementRules()){
        string repString = "\t";
        repString += rep.first + " -> '";
        for (const auto& sym: rep.second){
            repString += sym + " ";
        }
        repString.pop_back();
        repString += "'";
        cout << repString << endl;
    }
    cout << "}" << endl;
    output.emplace_back("}");

    // Startsymbool
    string startSym = "S = " + this->getStartSymbol();
    output.push_back(startSym);
    cout << startSym << endl;
    return output;
}
