#include "interactions.h"


int main() {

        PoolJoin* PJ = &PoolJoin::instance();
    //    MinerPopulation* MP = &MinerPopulation::instance();
    //    Pools* P = &Pools::instance();
        Game* G = &Game::instance();
        Stats* S = &Stats::instance();
    
        for (auto k=0; k<3; k++) {
            for (auto j=0; j<10; j++) {
                for (auto i=0; i<144; i++) {
                    G->mine();
                }
                PJ->run();
            }
        }
    
    
    
        S->printCurrentStats();
    //    MP->sort("rp");
    //    MP->printActiveMiners();
    //    P->print();
    
}
    

