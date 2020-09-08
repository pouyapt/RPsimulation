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
//    core::Random* gen = &core::Random::instance();
//    VirtualTime* V = &VirtualTime::instance();
//    MasterTime* M = &MasterTime::instance();
//    M->createNewModulator(0.1, 2, 3, 1800, 86400);
//    SineWaveModulator O(0.1, 2, 3, 1800, 86400);
//
//    for (int i=0; i<200000; i++) {
//        std::cout << O.apply() << std::endl;
//        V->addSecondsToCurrentTime(10);
//    }
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    Game* G = &Game::instance();
    for (auto j=0; j<14; j++) {
        //minerAndPoolsTest(MP, P);
        for (auto i=0; i<144; i++) {
            G->mine(1);
        }
    }
    
    
    //MP->sort("mp");
    MP->printActiveMiners();
    //P->print();
    printStats();
    
    
}

