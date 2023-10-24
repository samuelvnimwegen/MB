//
// Created by Samuel on 04/10/2023.
//

#ifndef MB_PRODUCTION_H
#define MB_PRODUCTION_H
#include "iostream"
#include "vector"
using namespace std;

class Production {
    string head;
    vector<string> body;
public:
    Production(string head, const vector<string> &body);

    [[nodiscard]] const string &getHead() const;

    [[nodiscard]] const vector<string> &getBody() const;

    [[nodiscard]] string getSortString() const;

};


#endif //MB_PRODUCTION_H
