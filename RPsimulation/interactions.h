#ifndef interactions_h
#define interactions_h

#include "structures.h"


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
    BW_Attack();
    ~BW_Attack();
    BW_Attack_Data* BW = &BW_Attack_Data::instance();
    core::Random* gen = &core::Random::instance();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    Stats* variableP = &Stats::instance();
    std::string filename = "Data/BW_Attack.db";
    bool assignVictim();
    bool selectMinerFromVictimPool();
    bool minerIsCorrupt(Miner* miner);
    int getBribedMiner(Miner* miner);
    Money calculateBribe(Money reward, Miner* miner);
    void read();
    void write();
    void CsvFileInit();
    void saveToCsvFile(int index, Money reward, Money bribe);
public:
    static BW_Attack& instance() {
        static BW_Attack instance;
        return instance;
    }
    bool initializeAttackEntities();
    bool processAttack(Miner* & miner, Money reward);
    void print();
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
    BW_Attack* BW = &BW_Attack::instance();
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
    int dishonestActivitiesCount = 0;
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
