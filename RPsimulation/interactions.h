#ifndef interactions_h
#define interactions_h

#include "structures.h"

class PoolMinerInteract {
private:
    MinerPopulation* MP = &MinerPopulation::instance(0);
    Pools* P = &Pools::instance();;
public:
    PoolMinerInteract();
};


class Game {
private:
    MinerPopulation* MP = &MinerPopulation::instance(0);
    Pools* P = &Pools::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    core::Random* gen = &core::Random::instance();
    void updateMinersPowerCost();
    Miner* winerMiner();
public:
    Game() {}
    void mine(int count=1);
};


#endif
