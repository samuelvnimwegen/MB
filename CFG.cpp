//
// Created by Samuel on 01/10/2023.
//

#include "CFG.h"

CFG::CFG() {
    init = this;

    // temp
    setVariables({"BINDIGIT", "S"});
    setTerminals({"0", "1", "a", "b"});
    setStartSymbol({"S"});
    vector<Production> replacements;
    auto prod1 = Production("BINDIGIT", {"0"});
    replacements.push_back(prod1);
    auto prod2 = Production("BINDIGIT", {"1"});
    replacements.push_back(prod2);
    auto prod3 = Production("S", {});
    replacements.push_back(prod3);
    auto prod4 = Production("S", {"a", "S", "b", "BINDIGIT"});
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
        this->addVariables(var);
    }
    vector<string> nonTerms = this->getVariables();
    std::sort(nonTerms.begin(), nonTerms.end());
    this->setVariables(nonTerms);

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
        this->addProduction(Production(head, body));
    }
    this->setProductions(sortProds(this->getProductions()));


    // Startsymbool toevoegen
    string start = j["Start"];
    this->setStartSymbol(start);
}

const vector<string> & CFG::getVariables() {
    assert(this->properlyInitialised());
    return variables;
}

void CFG::setVariables(const vector<string> &nonTerminals) {
    assert(this->properlyInitialised());
    assert(!nonTerminals.empty());
    CFG::variables = nonTerminals;
    assert(this->getVariables() == nonTerminals);
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

const vector<Production> & CFG::getProductions() {
    assert(this->properlyInitialised());
    assert(!this->productions.empty());
    return this->productions;
}

void CFG::setProductions(const vector<Production> &rules) {
    assert(this->properlyInitialised());
    assert(!rules.empty());
    this->productions = rules;
}

const string & CFG::getStartSymbol() {
    assert(this->properlyInitialised());
    assert(!startSymbol.empty());
    return startSymbol;
}

void CFG::setStartSymbol(const string &startSym) {
    assert(this->properlyInitialised());
    assert(!startSym.empty());
    assert(count(variables.begin(), variables.end(), startSym));
    CFG::startSymbol = startSym;
    assert(this->getStartSymbol() == startSym);

}

bool CFG::properlyInitialised() {
    return init == this;
}

vector<string> CFG::print() {
    this->setProductions(sortProds(this->getProductions()));
    this->setProductions(removeDupes(this->getProductions()));
    vector<string> output;

    // Non-terminals
    string nonTerms = "V = {";
    for (const auto& sym: this->getVariables()){
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
        string repString = "  ";
        repString += rep.getHead() + " -> `";
        for (const auto& sym: rep.getBody()){
            repString += sym + " ";
        }
        if (!rep.getBody().empty()){
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

void CFG::addVariables(const string &term) {
    assert(this->properlyInitialised());
    assert(!term.empty());
    assert(!std::count(this->getVariables().begin(), this->getVariables().end(), term));


    auto terms = this->getVariables();
    terms.push_back(term);
    this->setVariables(terms);
    assert(this->getVariables()[this->getVariables().size() - 1] == term);
}

void CFG::addProduction(const Production& prod) {
    assert(!prod.getHead().empty());
    assert(this->properlyInitialised());
    assert(std::count(this->getVariables().begin(), this->getVariables().end(), prod.getHead()));
    for (const auto& ch: prod.getBody()){
        bool charInSystem = std::count(this->getTerminals().begin(), this->getTerminals().end(),ch) or
                std::count(this->getVariables().begin(), this->getVariables().end(), ch);
        assert(charInSystem);
    }
    productions.push_back(prod);
}

CFG CFG::toCNF() {
    CFG CNF = CFG(this->filename)   ;
    cout << "Original CFG:" << endl << endl;
    CNF.print();
    cout << endl << "-------------------------------------" << endl << endl;

    cout << " >> Eliminating epsilon productions" << endl;
    CNF.removeNullable();
    cout <<"  Created " << CNF.getProductions().size() <<" productions, original had " << this->getProductions().size()
    << endl << endl;
    CNF.print();
    int productionAmount = int(CNF.getProductions().size());

    cout << endl << " >> Eliminating unit pairs" << endl;
    CNF.removeUnitPairs();
    cout << "  Created " << CNF.getProductions().size() << " new productions, original had " << productionAmount <<  endl << endl;
    CNF.print();
    cout << endl;
    productionAmount = int(CNF.getProductions().size());

    cout << " >> Eliminating useless symbols" << endl;
    CNF.removeUseless();
    cout << "  Removed " << this->getVariables().size() - CNF.getVariables().size() << " variables, " <<
    this->getTerminals().size() - CNF.getTerminals().size() << " terminals and " << productionAmount - CNF.getProductions().size()
    <<" productions" << endl << endl;
    CNF.print();
    cout << endl;
    productionAmount = int(CNF.getProductions().size());

    cout << " >> Replacing terminals in bad bodies" << endl;
    CNF.replaceBadBodies();
    cout << "  Created "<< CNF.getProductions().size() <<" new productions, original had " << productionAmount << endl << endl;
    CNF.print();
    cout << endl;
    productionAmount = int(CNF.getProductions().size());
    int varAmount = int(CNF.getVariables().size());

    CNF.breakBodies();
    cout << " >> Broke " << int(CNF.getProductions().size()) - productionAmount <<" bodies, added "<< int(
            CNF.getVariables().size()) - varAmount
    <<" new variables"<< endl;
    cout << ">>> Result CFG:" << endl << endl;
    CNF.print();


    return CNF;
}

void CFG::ll() {
    this->makeFirst();
    this->makeFollow();
}

void CFG::removeNullable() {
    // Vector met alle nullable terminals maken
    vector<string> nullableTerminals;
    for (const auto& terminal: this->getVariables()){
        if (isNullable({terminal})){
            nullableTerminals.push_back(terminal);
        }
    }
    // Nullables printen
    cout << "  Nullables are {";
    if (!nullableTerminals.empty()){
        for (int i = 0; i < nullableTerminals.size() - 1; ++i){
            cout << nullableTerminals[i] << ", ";
        }
        if (!nullableTerminals.empty()){
            cout << nullableTerminals[nullableTerminals.size() - 1];
        }
    }
    cout << "}" << endl;

    // Alle producties die op epsilon uitkomen verwijderen
    for (int i = 0; i < this->getProductions().size(); ++i){
        if (this->getProductions()[i].getBody().empty()){
            productions.erase(productions.begin() + i);
            --i;
        }
    }
    // Extra producties toevoegen
    for (const auto& production: this->getProductions()){
        // Alle subsets maken
        vector<vector<string>> subsets;
        makeNullableSubsets(subsets, nullableTerminals, production.getBody());

        // Als er meer dan 1 subset gemaakt kan worden, elk van deze bodies toevoegen.
        if (subsets.size() > 1){
            // Eerste body is al aanwezig, dus we beginnen bij de 2de (index 1)
            for (int i = 1; i < subsets.size(); ++i){
                addProduction(Production(production.getHead(), subsets[i]));
            }
        }
    }

    setProductions(removeDupes(sortProds(this->getProductions())));




}

bool CFG::isNullable(vector<string> inputString) {
    if (inputString.empty()){
        return true;
    }
    // Leftmost character van de string
    auto leftmost = inputString[0];
    for (const auto& production:getProductions()){
        // Als de productie uit de leftmost volgt en niet zichzelf bevat
        if (production.getHead() == leftmost and
                !std::count(production.getBody().begin(), production.getBody().end(), leftmost)){
            // We maken de nieuwe string: nieuwe body + inputString zonder leftmost variable
            auto newInput = production.getBody();
            newInput.insert(newInput.end(), inputString.begin() + 1, inputString.end());

            // Als deze nieuwe string nullable is: return true
            if (isNullable(newInput)){
                return true;
            }
        }
    }
    return false;
}

void CFG::removeUnitPairs() {
    // Aantal unit pairs bij start berekenen
    int n = 0;
    for (const auto& production: this->getProductions()){
        if (production.getBody().size() == 1 and
                std::count(this->getVariables().begin(), this->getVariables().end(), production.getBody()[0])){
            ++n;
        }
    }
    cout << "  Found " << n << " unit productions" << endl;

    // Alle reflexieve unitPairs toevoegen aan de vector met unit pairs.
    vector<pair<string, string>> unitPairs;
    for (const auto& nonTerm: this->getVariables()){
        unitPairs.emplace_back(nonTerm, nonTerm);
    }

    // Alle unit pairs wegwerken totdat ze allemaal weg zijn.
    bool done = false;
    while (!done){
        done = true;
        auto prods = this->getProductions();
        for (const auto& production : prods){
            // Als het de productie een unit productie is
            if (production.getBody().size() == 1 and
                    std::count(this->getVariables().begin(), this->getVariables().end(), production.getBody()[0])
                    and !std::count(unitPairs.begin(), unitPairs.end(),
                                    make_pair(production.getHead(), production.getBody()[0]))){
                unitPairs.emplace_back(production.getHead(), production.getBody()[0]);
                done = false;

                // Alle producties met hetzelfde head verzamelen.
                vector<Production> sameHeadProds;
                for (const auto& prd: this->getProductions()){
                    if (prd.getHead() == production.getBody()[0]){
                        sameHeadProds.push_back(prd);
                    }
                }

                // Alle nieuwe producties toevoegen
                for (const auto& prd: sameHeadProds){
                    auto newProd = Production(production.getHead(), prd.getBody());
                    this->addProduction(newProd);
                }
            }
        }
    }

    // Alle niet-unieke pairs verwijderen en de paren sorteren.
    unitPairs.erase(unique(unitPairs.begin(), unitPairs.end()), unitPairs.end());
    std::sort(unitPairs.begin(), unitPairs.end());

    cout << "  Unit pairs: {";
    for (const auto& unitProd:unitPairs){
        if (unitProd == unitPairs[unitPairs.size() - 1]){
            cout << "(" << unitProd.first << ", " << unitProd.second << ")}" << endl;
        }
        else{
            cout << "(" << unitProd.first << ", " << unitProd.second << "), ";
        }
    }

    // Alle unit producties verwijderen:
    vector<Production> newProds;
    for (const auto& production: this->getProductions()){
        assert(!production.getBody().empty());
        if (production.getBody().size() > 1 or std::count(this->getTerminals().begin(), this->getTerminals().end(), production.getBody()[0])){
            newProds.push_back(production);
        }
    }

    this->setProductions(removeDupes(sortProds(newProds)));
}

void CFG::removeUseless() {
    // Eerst zoeken we de generatingSymbols symbolen
    vector<string> generatingSymbols = this->getTerminals();
    bool done = false;
    while (!done){
        done = true;
        for (const auto& production: this->getProductions()){
            // Voor elke productie waarvan het head nog niet generatief is:
            if (!std::count(generatingSymbols.begin(), generatingSymbols.end(), production.getHead())){
                bool generative = true;
                for (const auto& var: production.getBody()){
                    // Als de var niet terminaal is en geen generatingSymbols symbol is, is deze productie niet generatief
                    if (!std::count(this->getTerminals().begin(), this->getTerminals().end(), var) and
                            !std::count(generatingSymbols.begin(), generatingSymbols.end(), var)){
                        generative = false;
                    }
                }
                if (generative){
                    generatingSymbols.push_back(production.getHead());
                    done = false;
                }
            }
        }
    }
    std::sort(generatingSymbols.begin(), generatingSymbols.end());

    // Nu de producties die een non-generatingSymbols symbool bevatten verwijderen
    vector<Production> newProds;
    for (const auto& production: this->getProductions()){
        bool generating = std::count(generatingSymbols.begin(), generatingSymbols.end(), production.getHead());
        for (const auto& sym: production.getBody()){
            if (!std::count(generatingSymbols.begin(), generatingSymbols.end(), sym)){
                generating = false;
            }
        }
        if (generating){
            newProds.push_back(production);
        }
    }
    setProductions(removeDupes(sortProds(newProds)));

    cout << "  Generating symbols: ";
    printVector(generatingSymbols);

    // Nu alle reachable symbolen zoeken
    vector<string> reachableSymbols = {this->getStartSymbol()};
    done = false;
    while (!done){
        done = true;
        for (const auto& production: this->getProductions()){
            // Als het head een reachable symbol is
            if (std::count(reachableSymbols.begin(), reachableSymbols.end(), production.getHead())){
                for (const auto& sym: production.getBody()){
                    // Voor elk symbool checken of deze al in de vector zit, anders toevoegen
                    if (!std::count(reachableSymbols.begin(), reachableSymbols.end(), sym)){
                        reachableSymbols.push_back(sym);
                        done = false;
                    }
                }
            }
        }
    }

    std::sort(reachableSymbols.begin(), reachableSymbols.end());

    // Nu alle producties met unreachable symbolen weghalen
    newProds = {};
    for (const auto& production: this->getProductions()){
        // Kijken of head reachable is:
        bool reachable = std::count(reachableSymbols.begin(), reachableSymbols.end(), production.getHead());
        for (const auto& sym: production.getBody()){
            // Als 1 van de body symbolen niet reachable is, dan productie niet reachable.
            if (!std::count(reachableSymbols.begin(), reachableSymbols.end(), sym)){
                reachable = false;
            }
        }
        if (reachable){
            newProds.push_back(production);
        }
    }
    setProductions(removeDupes(sortProds(newProds)));

    cout << "  Reachable symbols: ";
    printVector(reachableSymbols);

    cout << "  Useful symbols: ";
    printVector(reachableSymbols);

    vector<string> newTerms, newNonTerms;
    for (const auto& sym: this->getTerminals()){
        if (std::count(reachableSymbols.begin(), reachableSymbols.end(), sym)){
            newTerms.push_back(sym);
        }
    }
    for (const auto& sym: this->getVariables()){
        if (std::count(reachableSymbols.begin(), reachableSymbols.end(), sym)){
            newNonTerms.push_back(sym);
        }
    }

    this->setTerminals(newTerms);
    this->setVariables(newNonTerms);
}

void CFG::replaceBadBodies() {
    // Beginnen met alle bodies >= 2 met alleen variabelen te schrijven.
    vector<Production> newProds;
    vector<string> newVars = this->getVariables();
    vector<string> newVarsWithoutOld;
    for (const auto& production: this->getProductions()){
        if (production.getBody().size() >= 2){
            vector<string> newBody;
            for (const auto& sym: production.getBody()){
                // Als er een symbool is dat een terminal is:
                if (std::count(this->getTerminals().begin(), this->getTerminals().end(), sym)){
                    bool alreadyProd = false;
                    for (const auto& prd: this->getProductions()){
                        // Als er al een productie is de rechtstreeks op het symbool uitkomt gebruiken we deze
                        if (prd.getBody().size() == 1 and prd.getBody()[0] == sym){
                            newBody.push_back(prd.getHead());
                            alreadyProd = true;
                        }
                    }
                    // Als er nog geen productie is in de vorige CFG, _x toevoegen aan de nieuwe body.
                    if (!alreadyProd){
                        newBody.push_back("_" + sym);
                        // Als de variabele nog niet aangemaakt is: deze aanmaken samen met de productie
                        if (!std::count(newVars.begin(), newVars.end(), "_" + sym)){
                            newProds.push_back(Production("_" + sym, {sym}));
                            newVars.push_back("_" + sym);
                            newVarsWithoutOld.push_back("_" + sym);
                        }
                    }
                }
                // Als het symbool een variabele is
                else{
                    newBody.push_back(sym);
                }
            }
            newProds.emplace_back(production.getHead(), newBody);
        }
        else{
            newProds.push_back(production);
        }
    }
    this->setProductions(removeDupes(sortProds(newProds)));
    std::sort(newVars.begin(), newVars.end());
    this->setVariables(newVars);
    std::sort(newVars.begin(), newVars.end());
    cout << "  Added "<< newVarsWithoutOld.size() <<" new variables: ";
    printVector(newVarsWithoutOld);
}

void CFG::breakBodies() {
    vector<string> newVars = getVariables();
    bool klaar = false;
    vector<Production> newProds;
    while (!klaar){
        klaar = true;
        for (const auto& production: this->getProductions()){
            if (production.getBody().size() >= 3){
                klaar = false;
                // Nieuwe variabele aanmaken:
                string varName = breakBodyName(production.getHead(), newVars);
                newVars.push_back(varName);

                // De 2 nieuwe producties aanmaken
                newProds.push_back(Production(production.getHead(), {production.getBody()[0], varName}));
                vector<string> newBody(production.getBody().begin() + 1, production.getBody().end());
                newProds.emplace_back(varName, newBody);
            }
            else{
                newProds.push_back(production);
            }
        }

        // Bij elke while loop iteratie de producties updaten
        setProductions(removeDupes(sortProds(newProds)));
        newProds.clear();
    }
    std::sort(newVars.begin(), newVars.end());
    setVariables(newVars);
}


const vector<Production> &CFG::getFirst() const {
    return first;
}

void CFG::setFirst(const vector<Production> &fr) {
    CFG::first = fr;
}

const vector<Production> &CFG::getFollow() const {
    return follow;
}

void CFG::setFollow(const vector<Production> &fl) {
    CFG::follow = fl;
}

void CFG::makeFirst() {
    // Eerst maken we alle firstProductions
    vector<Production> firstProds;
    vector<Production> current = this->getProductions();
    while (!current.empty()){
        vector<Production> next;
        for (const auto& production: current){
            // Als het body van de operatie leeg is: Een spatie toevoegen
            if (production.getBody().empty()){
                firstProds.push_back(Production(production.getHead(), {"|"}));
            }
            // Als het eerste char van de body een terminal is: dan is dit de opl
            else if (std::count(this->getTerminals().begin(), this->getTerminals().end(), production.getBody()[0])){
                firstProds.push_back(Production(production.getHead() ,{production.getBody()[0]}));
            }
            // Als het eerste char een variabele is
            else if (std::count(this->getVariables().begin(), this->getVariables().end(), production.getBody()[0])){
                vector<Production> newProds;
                for (const auto& firstProd: firstProds){
                    // Als de variabele gevonden is: de body van deze variabele als body gebruiken
                    if (firstProd.getHead() == production.getBody()[0]){
                        newProds.push_back(Production(production.getHead(), {firstProd.getBody()[0]}));
                    }
                }
                // Als er geen oplossingen zijn gevonden: toevoegen aan next
                if (newProds.empty()){
                    next.push_back(production);
                }
                // Als er wel oplossingen zijn gevonden, deze aan firstProds toevoegen
                else{
                    for (const auto& prd: newProds){
                        firstProds.push_back(prd);
                    }
                }
            }
            else{
                cerr << "variabele niet herkend: " << production.getBody()[0] << endl;
            }
        }
        // Alle niet gevonden variabelen terug in de loop zetten
        current = next;
    }

    // Nu we alle producties hebben gaan we alle producties mergen die hetzelfde head hebben
    vector<Production> firstProductMerged;
    vector<string> heads;
    for (const auto& production: firstProds){
        // Checken of head niet al eerder uitgewerkt is
        if (!std::count(heads.begin(), heads.end(), production.getHead())){
            auto mergedBody = production.getBody();
            for (const auto& brotherProduction: firstProds){
                if (production.getHead() == brotherProduction.getHead() and production.getBody() != brotherProduction.getBody()){
                    mergedBody.insert(mergedBody.end(), brotherProduction.getBody().begin(), brotherProduction.getBody().end());
                }
            }
            std::sort(mergedBody.begin(), mergedBody.end());
            firstProductMerged.emplace_back(production.getHead(), mergedBody);
            heads.push_back(production.getHead());
        }
    }
    setFirst(removeDupes(sortProds(firstProductMerged)));
}

void CFG::makeFollow() {
    // Alle variabelen afgaan en follow zoeken tot dat elke variabele een 'follow' heeft
    vector<string> case3CompletedVars;
    bool done = false;
    int loopCounter = 0;
    while (!done){
        assert(loopCounter <= int(this->getVariables().size()));
        done = true;
        for (const auto& variable: this->getVariables()){
            vector<Production> followProds;
            // Case 1: het hoofd is het startsymbool:
            if (variable == this->getStartSymbol()){
                followProds.push_back(Production(variable, {"<EOS>"}));
            }

            // Case 2: Er is een productie waarin het hoofd niet het laatste symbool is, maar wel voor komt
            for (const auto& production: this->getProductions()){
                if (std::count(production.getBody().begin(), production.getBody().end(), production.getHead())
                    and production.getBody()[production.getBody().size() - 1] != variable){
                    // De index opzoeken van het volgende element in de string waarin het symbool voor komt
                    int indexNext = -1;
                    for (int i = 0; i < production.getBody().size(); ++i){
                        if (production.getBody()[i] == variable){
                            indexNext = i + 1;
                        }
                    }
                    assert(indexNext != -1);
                    auto nextSymbolInString = production.getBody()[indexNext];

                    // Als het een terminal is, deze invoeren als follow
                    if (std::count(this->getTerminals().begin(), this->getTerminals().end(), nextSymbolInString)){
                        followProds.push_back(Production(variable, {nextSymbolInString}));
                    }

                    // Als het een variable is
                    else{
                        assert(std::count(this->getVariables().begin(), this->getVariables().end(), nextSymbolInString));
                        // De 'first' productie zoeken met dit volgende symbool als head en de body hiervan opslaan
                        for (const auto& firstProd: this->getFirst()){
                            if (firstProd.getHead() == nextSymbolInString){
                                followProds.emplace_back(variable, firstProd.getBody());
                            }
                        }
                    }
                }
            }

            // Case 3: Als de variabele voor komt in een andere productie als laatste variabele, waarvan de variabele niet het head is
            bool case3 = true;
            for (const auto& production: this->getProductions()){
                if (production.getHead() != variable and production.getBody().size() >= 2 and production.getBody()[production.getBody().size() - 1] == variable){
                    // Als het head al een 'follow' heeft:
                    if (std::count(case3CompletedVars.begin(), case3CompletedVars.end(), production.getHead())){
                        // De 'follow' van het head zoeken:
                        vector<string> followBody;
                        for (const auto& foll: this->getFollow()){
                            if (foll.getHead() == production.getHead()){
                                followBody = foll.getBody();
                            }
                        }
                        followProds.emplace_back(variable, followBody);
                    }
                    // Als deze head nog geen 'follow' heeft, de hele loop nog eens runnen
                    else{
                        done = false;
                        case3 = false;
                    }
                }
            }

            // Case 4: Als er een productie is van size 3 in de vorm A → (BCD) waarbij epsilon in de 'first' van D zit,
            // dan wordt er een extra body aan Follow(C) gegeven namelijk Follow(A)
            bool case4 = true;
            for (const auto& production: this->getProductions()){
                bool lastSymIsVar = std::count(this->getVariables().begin(), this->getVariables().end(), production.getBody()[2]);
                if (production.getBody().size() == 3 and production.getBody()[1] == variable and lastSymIsVar){
                    // Eerst zoeken we de 'first' van het symbool na de huidige variabele
                    vector<string> firstProdBody;
                    for (const auto& firstPrd: this->getFirst()){
                        if (firstPrd.getHead() == production.getBody()[production.getBody().size() - 1]){
                            firstProdBody = firstPrd.getBody();
                        }
                    }
                    assert(!firstProdBody.empty());

                    // Nu kijken we of epsilon hierin zit of in dit geval "|" wat hier als epsilon gebruikt wordt
                    bool epsilonInVec = std::count(firstProdBody.begin(), firstProdBody.end(),"|");

                    // Nu checken of er al een 'follow' aangemaakt met het head van de huidige productie
                    bool followExists = false;
                    vector<string> followBody;
                    for (const auto& followProd: this->getFollow()){
                        if (followProd.getHead() == production.getHead()){
                            followExists = true;
                            followBody = followProd.getBody();
                        }
                    }
                    assert(followExists or followBody.empty());

                    // Als deze 'follow' nog niet bestaat, de loop opnieuw runnen en case 4 kan nog niet uitgevoerd worden
                    if (!followExists){
                        case4 = false;
                        done = false;
                    }

                    // Als deze allebei gelden
                    else if (epsilonInVec and followExists){
                        followProds.emplace_back(variable, followBody);
                    }
                }
            }

            // Checken of alle cases geen fouten zijn tegengekomen:
            if (case4 and case3){
                // Alle productions mergen en toevoegen aan Follow()
                Production mergedProduction = mergeProductions(followProds);
                auto newFollow = this->getFollow();
                newFollow.push_back(mergedProduction);
                setFollow(newFollow);
            }
        }
        ++loopCounter;
    }
}

vector<Production> sortProds(const vector<Production> &prods) {
    vector<string> sortStrings;
    for (const auto& production: prods){
        sortStrings.push_back(production.getSortString());
    }

    std::sort(sortStrings.begin(), sortStrings.end());

    vector<Production> newProds;
    for (const auto& str: sortStrings){
        for (const auto& prod: prods){
            if (prod.getSortString() == str){
                newProds.push_back(prod);
            }
        }
    }
    return newProds;
}

vector<Production> removeDupes(const vector<Production> &prods){
    vector<Production> newProds;
    newProds.push_back(prods[0]);
    for (const auto& production: prods){
        if (production.getHead() != newProds[newProds.size() - 1].getHead() or production.getBody() != newProds[newProds.size() - 1].getBody()){
            newProds.push_back(production);
        }
    }
    return newProds;
}
void makeNullableSubsets(vector<vector<string>> &subsets, const vector<string> &nullables, const vector<string> &current) {
    assert(!current.empty());
    // Checken of current niet al in de subsets verzameling zit
    for (const auto& subset: subsets){
        if (subset == current){
            return;
        }
    }
    subsets.push_back(current);
    if (current.size() == 1){
        return;
    }

    for (int i = 0; i < current.size(); ++i){
        // Als het character nullable is
        if (std::count(nullables.begin(), nullables.end(), current[i])){
            auto copy = current;
            copy.erase(copy.begin() + i);
            makeNullableSubsets(subsets, nullables, copy);
        }
    }
}

void printVector(const vector<string> &items) {
    string outString = "{";
    for (const auto& sym: items){
        outString += sym + ", ";
    }
    if (!items.empty()){
        outString.pop_back();
        outString.pop_back();
    }
    outString += "}";
    cout << outString << endl;
}

string breakBodyName(const string &head, const vector<string> &vars) {
    int i = 2;
    while (std::count(vars.begin(), vars.end(), head + "_" + to_string(i))){
        ++i;
    }
    return head + "_" + to_string(i);
}

Production mergeProductions(const vector<Production> &productions) {
    assert(!productions.empty());
    string productionHead = productions[0].getHead();
    vector<string> mergedBody;
    for (const auto& production: productions){
        mergedBody.insert(mergedBody.end(), production.getBody().begin(), production.getBody().end());
    }
    Production result = Production(productionHead, mergedBody);
    return result;
}
