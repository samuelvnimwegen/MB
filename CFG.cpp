//
// Created by Samuel on 01/10/2023.
//

#include "CFG.h"

CFG::CFG() {
    init = this;
}


const vector<string> & CFG::getNonTerminals() {
    assert(this->properlyInitialised());
    auto result = terminals;
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

const map<string, string> & CFG::getReplacementRules() {
    assert(this->properlyInitialised());
    return replacementRules;
}

void CFG::setReplacementRules(const map<string, string> &rules) {
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
    CFG::startSymbol = startSym;
    assert(this->getStartSymbol() == startSym);
}

bool CFG::properlyInitialised() {
    return init == this;
}
