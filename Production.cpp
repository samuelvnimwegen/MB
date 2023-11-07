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
    out += " -> `";

    for (const auto& ch: this->getBody()){
        out += ch + " ";
    }
    if (!getBody().empty()){
        out.pop_back();
    }
    out += "`";
    return out;
}

string Production::getBodyString() const {
    // Als het body leeg of epsilon is, lege string return
    if (this->getBody().empty() or (this->getBody().size() == 1 and this->getBody().at(0) == "|")){
        return "";
    }

    string bodyString = "`";
    for (const auto& bodySym: this->getBody()){
        bodyString += bodySym + " ";
    }
    bodyString.pop_back();
    bodyString += "`";

    return bodyString;
}


