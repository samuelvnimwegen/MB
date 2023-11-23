#include "CFG.h"

using namespace std;

int main() {
    CFG cfg("input-cnf1.json");
    cfg.toCNF();
    return 0;
}
