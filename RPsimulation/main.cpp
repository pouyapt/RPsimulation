#include "interactions.h"

void minerAndPoolsTest(MinerPopulation &DB, Pools &P) {
    int n = 1951;
    while (n) {
        for (int i=0; i<P.size(); i++)
            P[i]->pickMiner(DB[rand()]);
        n--;
    }
}


int main() {
    MinerPopulation* P = &MinerPopulation::instance(0);
    P->print();
}

