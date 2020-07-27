#include "structures.h"

int main() {
    MinerPopulation DB(0);
    Pools P(DB);
//    for (int i=0; i<P.size(); i++) {
//        int n = 265;
//        while (n) {
//            P[i]->pickMiner(DB[rand()]);
//            n--;
//        }
//    }
    
    DB.print();
    
}

