#include "interactions.h"


void minerAndPoolsTest(MinerPopulation* MP, Pools* P) {
    int n = 100;
    while (n) {
        for (int i=0; i<P->size(); i++) {
            unsigned k = rand();
            (*P)[i]->pickMiner((*MP)[k]);
        }
        n--;
    }
}

int main() {

//    Stats* stats = &Stats::instance();
//    MinerPopulation* MP = &MinerPopulation::instance();
//    Pools* P = &Pools::instance();
//    Game* G = &Game::instance();
//    for (auto j=0; j<5; j++) {
//        minerAndPoolsTest(MP, P);
//        for (auto i=0; i<144; i++) {
//            G->mine();
//        }
//    }
    
    //MP->sort("mp");
    //MP->printActiveMiners();
    //P->print();
    //stats->printCurrentStats();
}

