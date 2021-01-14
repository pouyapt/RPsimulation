#ifndef interactions_h
#define interactions_h

#include "structures.h"

struct AttackGroup {
    AttackGroup() {
        victim = nullptr;
        suspect = nullptr;
        bribedMiner = nullptr;
        winnerMiner = nullptr;
    }
    PoolManager* victim;
    PoolManager* suspect;
    Miner* bribedMiner;
    Miner* winnerMiner;
};

class PoolJoin {
public:
    static PoolJoin& instance() {
        static PoolJoin instance;
        return instance;
    }
    void run();
private:
    int countUntilProcessInvitations;
    void generateCount();
    PoolJoin();
    ~PoolJoin();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    core::Random* gen = &core::Random::instance();
    Stats* stats = &Stats::instance();
    void writeFile();
    bool readFile();
    std::string file = "Data/pooljoin.db";
    void provideMiners();
    void sendInvitations();
    void processInvitations();
    void clearCandidateMinersList();
};

//--------------------------------------------------------------------------------

class BW_Attack {
private:
    core::list<AttackGroup> list;
    core::Random* gen = &core::Random::instance();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    bool assignVictim_Suspect();
    bool selectMinerFromVictimPool();
    bool selectMinerFromSuspectPool();
    bool minerIsCorrupt(Miner* miner);
    int getBribedMiner(Miner* miner);
    Money calculateBribe(Money reward, Miner* miner);
public:
    bool initializeAttackEntities();
    bool processAttack(Miner* miner, Money reward);
};

//---------------------------------------------------------------------------------

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
    void updateCosts();
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
    void updateModulatedUnitPrice();
    void generateInitialUnitPrice();
    double costRewardRatio(long population);
public:
    static Game& instance() {
        static Game instance;
        return instance;
    }
    void mine();
    Time lastGeneratedBlockTime() const;
};


#endif
