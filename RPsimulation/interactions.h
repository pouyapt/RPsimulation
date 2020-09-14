#ifndef interactions_h
#define interactions_h

#include "structures.h"

class PoolMinerInteract {
private:
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    int poolMiners = 0;
public:
    PoolMinerInteract();
};


class Game {
private:
    Game();
    ~Game();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    Stats* variableP = &Stats::instance();
    PopulationParameters* populationP = &PopulationParameters::instance();
    MasterTime* T = &MasterTime::instance();
    core::Random* gen = &core::Random::instance();
    void updateMinersPowerCost();
    int priceModulatorIndex = 0;
    Miner* winerMiner();
    Money totalNetworkRevenue;
    Money totalNetworkCosts;
    Money unitPrice;
    Money modulatedUnitPrice;
    int unitsPerBlock = 20;
    int totalMinedBlocks;
    double currentCostRewardRatio;
    std::string file = "Data/game.db";
    Time lastRoundDuration;
    Time lastGeneratedBlockTimestamp;
    Money lastRoundPowerCost;
    bool ReadGameFile();
    void WriteGameFile();
    void updateVariableParameters();
    void updateUnitPrice();
    void generateInitialUnitPrice();
    void modulateUnitPrice();
public:
    static Game& instance() {
        static Game instance;
        return instance;
    }
    void mine();
    Time lastGeneratedBlockTime() const;
};


#endif
