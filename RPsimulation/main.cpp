#include "interactions.h"



void minerAndPoolsTest(MinerPopulation* MP, Pools* P) {
    int n = 1951;
    while (n) {
        for (int i=0; i<P->size(); i++)
            (*P)[i]->pickMiner((*MP)[rand()]);
        n--;
    }
}




int main() {
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    //minerAndPoolsTest(MP, P);
    Game* G = &Game::instance();
    G->mine(1);
    //MP->sort("mc");
    //MP->printActiveMiners();
    P->print();
    printStats();
}

