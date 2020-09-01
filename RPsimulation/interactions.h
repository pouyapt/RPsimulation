#ifndef interactions_h
#define interactions_h

#include "structures.h"

class PoolMinerInteract {
private:
    MinerPopulation* MP = &MinerPopulation::instance(0);
    Pools* P = &Pools::instance();
    int poolMiners = 0;
public:
    PoolMinerInteract();
};


class Game {
private:
    Game();
    ~Game();
    MinerPopulation* MP = &MinerPopulation::instance(0);
    Pools* P = &Pools::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    VariableParameters* variableP = &VariableParameters::instance();
    core::Random* gen = &core::Random::instance();
    void updateMinersPowerCost();
    Miner* winerMiner();
    Money totalNetworkRevenue;
    Money totalNetworkCosts;
    int totalMinedBlocks;
    std::string file;
    Time lastRoundDuration;
    Money lastRoundPowerCost;
    bool ReadGameFile();
    void WriteGameFile();
    void updateVariableParameters();
    void updateUnitPrice();
public:
    static Game& instance() {
        static Game instance;
        return instance;
    }
    void mine(int count=1);
};


#endif
