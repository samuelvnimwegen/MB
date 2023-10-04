//
// Created by Samuel on 04/10/2023.
//

#include "Production.h"

#include <utility>

Production::Production(string head, const vector<string> &body) : head(std::move(head)), body(body) {}

const string &Production::getHead() const {
    return head;
}

const vector<string> &Production::getBody() const {
    return body;
}

string Production::getSortString() const {
    string out = this->getHead();
    for (const auto& ch: this->getBody()){
        out += ch;
    }
    return out;
}
