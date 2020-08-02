#include "game.h"

void minerAndPools(MinerPopulation &DB, Pools &P) {
    int n = 1251;
    while (n) {
        for (int i=0; i<P.size(); i++)
            P[i]->pickMiner(DB[rand()]);
        n--;
    }
}

int main() {
    
    MinerPopulation DB(25300);
    Pools P(DB);
    minerAndPools(DB, P);
    Game G(DB);
    G.mine(850);
    DB.sort("pf");
    DB.print();
    
}

