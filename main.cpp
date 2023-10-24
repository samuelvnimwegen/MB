#include "CFG.h"

int main() {
    CFG cfg("input-cnf1.json");
    cfg.toCNF();
    return 0;
}
