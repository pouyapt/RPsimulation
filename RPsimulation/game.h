#ifndef game_h
#define game_h

#include "structures.h"


class Game {
private:
    MinerPopulation* DB;
    void updateMinersPowerCost();
    Miner* winerMiner();
public:
    Game(MinerPopulation & DB);
    void mine(int count=1);
};


#endif
