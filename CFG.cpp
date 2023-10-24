//
// Created by Samuel on 01/10/2023.
//

#include "CFG.h"

CFG::CFG() {
    init = this;

    // temp
    setNonTerminals({"BINDIGIT", "S"});
    setTerminals({"0", "1", "a", "b"});
    setStartSymbol({"S"});
    vector<Production*> replacements;
    auto prod1 = new Production("BINDIGIT", {"0"});
    replacements.push_back(prod1);
    auto prod2 = new Production("BINDIGIT", {"1"});
    replacements.push_back(prod2);
    auto prod3 = new Production("S", {});
    replacements.push_back(prod3);
    auto prod4 = new Production("S", {"a", "S", "b", "BINDIGIT"});
    replacements.push_back(prod4);
    setProductions(replacements);
}

CFG::CFG(const string &name){
    init = this;

    // Input checken en invoeren
    assert(!name.empty());
    assert(name.substr(name.size() - 5) == ".json");
    this->filename = name;
    assert(this->filename == name);
    ifstream input(name.c_str());
    assert(input.good());
    json j;
    input >> j;
    assert(!j.empty());

    // Alle variables toevoegen
    for (string var: j["Variables"]){
        this->addNonTerminal(var);
    }
    vector<string> nonTerms = this->getNonTerminals();
    std::sort(nonTerms.begin(), nonTerms.end());
    this->setNonTerminals(nonTerms);

    // Alle Terminals toevoegen
    for (string term: j["Terminals"]){
        this->addTerminal(term);
    }
    auto terms = this->getTerminals();
    std::sort(terms.begin(), terms.end());
    this->setTerminals(terms);

    // Alle productions toevoegen
    for (auto prod: j["Productions"]){
        string head = prod["head"];
        vector<string> body;
        for (string ch: prod["body"]){
            body.push_back(ch);
        }
        this->addProduction(new Production(head, body));
    }
    this->setProductions(sortProds(this->getProductions()));


    // Startsymbool toevoegen
    string start = j["Start"];
    this->setStartSymbol(start);
}


const vector<string> & CFG::getNonTerminals() {
    assert(this->properlyInitialised());
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
    return terminals;
}

void CFG::setTerminals(const vector<string> &term) {
    assert(this->properlyInitialised());
    assert(!term.empty());
    CFG::terminals = term;
    assert(this->getTerminals() == term);
}

const vector<Production*> & CFG::getProductions() {
    assert(this->properlyInitialised());
    assert(!this->productions.empty());
    return this->productions;
}

void CFG::setProductions(const vector<Production*> &rules) {
    assert(this->properlyInitialised());
    assert(!rules.empty());
    this->productions = rules;
    assert(this->getProductions() == rules);
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
    for (const auto& rep: this->getProductions()){
        string repString = "    ";
        repString += rep->getHead() + " -> `";
        for (const auto& sym: rep->getBody()){
            repString += sym + " ";
        }
        if (!rep->getBody().empty()){
            repString.pop_back();
        }
        repString += "`";
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

void CFG::addTerminal(const string &term) {
    assert(this->properlyInitialised());
    assert(!term.empty());
    assert(this->getTerminals().empty() or !std::count(this->getTerminals().begin(), this->getTerminals().end(), term));
    auto terms = this->getTerminals();
    terms.push_back(term);
    this->setTerminals(terms);
    assert(this->getTerminals()[this->getTerminals().size() - 1] == term);
}

void CFG::addNonTerminal(const string &term) {
    assert(this->properlyInitialised());
    assert(!term.empty());
    assert(!std::count(this->getNonTerminals().begin(), this->getNonTerminals().end(), term));


    auto terms = this->getNonTerminals();
    terms.push_back(term);
    this->setNonTerminals(terms);
    assert(this->getNonTerminals()[this->getNonTerminals().size() - 1] == term);
}

void CFG::addProduction(Production *prod) {
    assert(this->properlyInitialised());
    assert(std::count(this->getNonTerminals().begin(), this->getNonTerminals().end(),prod->getHead()));
    for (const auto& ch: prod->getBody()){
        bool charInSystem = std::count(this->getTerminals().begin(), this->getTerminals().end(),ch) or
                std::count(this->getNonTerminals().begin(), this->getNonTerminals().end(), ch);
        assert(charInSystem);
    }
    this->productions.push_back(prod);
    assert(this->getProductions()[this->getProductions().size() - 1] == prod);
}

CFG CFG::toCNF() {
    CFG CNF = CFG(this->filename)   ;
    cout << "Original CFG:" << endl << endl;
    CNF.print();
    cout << endl << "-------------------------------------" << endl << endl;
    cout << " >> Eliminating epsilon productions" << endl;

    return CNF;
}

vector<Production *> sortProds(const vector<Production*>& prods) {
    vector<string> sortStrings;
    for (auto production: prods){
        sortStrings.push_back(production->getSortString());
    }
    sort(sortStrings.begin(), sortStrings.end());

    vector<Production*> newProds;
    for (const auto& str: sortStrings){
        for (auto prod: prods){
            if (prod->getSortString() == str){
                newProds.push_back(prod);
            }
        }
    }
    return newProds;
}
