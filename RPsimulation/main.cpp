#include "game.h"

void minerAndPools(MinerPopulation &DB, Pools &P) {
    for (int i=0; i<P.size(); i++) {
        int n = 265;
        while (n) {
            P[i]->pickMiner(DB[rand()]);
            n--;
        }
    }
}


int main() {
    MinerPopulation DB(0);
    Pools P(DB);
    //minerAndPools(DB, P);
    for (int i=0; i<0; i++) {
        mineGame(DB);
    }
    //DB.sort("pf");
    P.print();
}

