#include "interactions.h"


int main() {
    
    
    PoolJoin* PJ = &PoolJoin::instance();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    Game* G = &Game::instance();
    Stats* S = &Stats::instance();
    for (auto k=0; k<6; k++) {
        for (auto j=0; j<30; j++) {
            for (auto i=0; i<144; i++) {
                G->mine();
            }
            PJ->run();
        }
    }

//    for (auto i=0; i<1; i++) {
//        G->mine();
//    }

    P->print();
    S->printCurrentStats();
//    MP->sort("rp");
//    MP->printActiveMiners();
    

}
    

