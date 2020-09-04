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
    core::Random* gen = &core::Random::instance();
    VirtualTime* V = &VirtualTime::instance();
    
    SineWaveModulator O(0.36, 2, 3, 2400, 604800);

    for (int i=0; i<30000; i++) {
        std::cout << O.apply() << std::endl;
        V->addSecondsToCurrentTime(60);
    }
    
    
    //MinerPopulation* MP = &MinerPopulation::instance(10);
    //Pools* P = &Pools::instance();
    //minerAndPoolsTest(MP, P);
    //Game* G = &Game::instance();
    //G->mine(1);
    //MP->sort("mc");
    //MP->printActiveMiners();
    //P->print();
    //printStats();
}

