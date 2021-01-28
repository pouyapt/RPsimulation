#ifndef interactions_h
#define interactions_h

#include "structures.h"


class PoolJoin {
public:
    static PoolJoin& instance() {
        static PoolJoin instance;
        return instance;
    }
    void runReputationMode();
    void runNormalMode();
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
    BwAttackParameters* BWP = &BwAttackParameters::instance();
    BW_Attack_Data* BW = &BW_Attack_Data::instance();
    core::Random* gen = &core::Random::instance();
    MinerPopulation* MP = &MinerPopulation::instance();
    Pools* P = &Pools::instance();
    Stats* variableP = &Stats::instance();
    std::string filename = "Data/BW_Attack.db";
    bool assignVictim();
    bool selectMinerFromVictimPool();
    bool minerIsCorrupt(Miner* miner, double & bribePercentage, int & numberOfRounds);
    int getBribedMiner(Miner* miner);
    double calculateBribePercentage();
    bool checkMinerHashShare(Miner* miner, PoolManager* p);
    void read();
    void write();
    void CsvFileInit();
    void saveToCsvFile(int index, double reward, double bribe);
public:
    static BW_Attack& instance() {
        static BW_Attack instance;
        return instance;
    }
    bool initializeAttackEntities();
    bool processAttack(Miner* miner, double reward, int & activity);
    void print();
};

//---------------------------------------------------------------------------------

class Reputation {
private:
    MinerPopulation* MP = &MinerPopulation::instance();
    MasterTime* T = &MasterTime::instance();
    Stats* variableP = &Stats::instance();
    double minerPresenceDurationInYear(Miner* miner);
    void updateHighestLowestReputation(Miner* miner);
public:
    Reputation() {}
    ~Reputation() {}
    void applyNegativeReputation(Miner* miner);
    void updateReputation(Miner* miner);
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
    Reputation R;
    void updateMiningEntitiesData();
    int priceModulatorIndex = 0;
    Miner* winerMiner();
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
    int dishonestActivityCount = 0;
    int detectedDishonestActivityCount = 0;
    int falseDetectedDishonestActivityCount = 0;
    bool ReadGameFile();
    void WriteGameFile();
    void updateVariableParameters();
    void updateUnitPrice();
    void updateModulatedUnitPrice();
    void generateInitialUnitPrice();
    double costRewardRatio(long population);
    void updateExpectedMinedBlocks(Miner* miner);
    void updateExpectedMinedBlocks(PoolManager* p);
    void processBwDetection(PoolManager* P);
public:
    static Game& instance() {
        static Game instance;
        return instance;
    }
    void mine();
    Time lastGeneratedBlockTime() const;
};


#endif
