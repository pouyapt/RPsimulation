#ifndef structures_hpp
#define structures_hpp

#include "BaseEntities.h"


class MinerPopulation {
private:
    MinerPopulation();
    ~MinerPopulation();
    long totalHashPower_ = 0;
    int allViolationsCount = 0;
    int detectedViolationsCount = 0;
    double highestMinerReutation = 0;
    double lowestMinerReutation = 0;
    long removedMinersCount = 0;
    int removeLostMinersCountdown = 0;
    double population = 0;
    bool populationStage = 0;
    char populationDataFile[30] = "Data/p_data.db";
    char minersDataFile[30] = "Data/p_list.db";
    char removedMinersDataFile[30] = "Data/p_list_removed.db";
    void writeMinersData(std::ofstream &out);
    void readMinersData(std::ifstream &in, int size);
    void writePopulationData();
    void resetGlobalParameters();
    bool readPopulationData();
    void writeRemovedMiners();
    void addMiner(long time=0);
    void deleteMiner(unsigned index);
    void removeMinerFromPool(Miner* miner);
    void shuffleValueGen();
    void saveOldOrder();
    void updateVariableParameters();
    double processPopulationChange();
    PopulationParameters* populationP = &PopulationParameters::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    core::Random* gen = &core::Random::instance();
    Stats* variableP = &Stats::instance();
    MasterTime* T = &MasterTime::instance();
    int calculateInitialPopulation();
    void calculateNumberOfNewMiners(long time);
    void processMinersAddition(long time);
    void removeLostMiners();
    void processMinersRemoval();
    double populationEstimate(long time);
    double populationGrowthPhase1(long seconds);
    double populationGrowthPhase2(long population);
    void updateReputation(Miner* miner);
    void updateAllReputations();
    double minerPresenceDurationInYear(Miner* miner);
    void updateHighestLowestReputation(Miner* miner);
    core::list<Miner*> allMinersList;
    core::list<Miner*> removedList;
public:
    static MinerPopulation& instance() {
        static MinerPopulation instance;
        return instance;
    }
    MinerPopulation(MinerPopulation &orig) = delete;
    unsigned int size();
    Miner* operator [] (unsigned index);
    MinerPopulation operator=(MinerPopulation &orig) = delete;
    long totalHashPower();
    int getAllViolationsCount();
    int& getDetectedViolationsCount();
    void saveIndex();
    void sort(std::string by="");
    void shuffleList();
    void undoShuffleList();
    void printActiveMiners();
    void printPopulationStat();
    void writeMinersInvitations(Pools* P);
    void readMinersInvitations(Pools* P);
    void populationUpdate(long time, const Game& game);
    int topHashPower();
    int MinersWithAtLeastOneBlock();
    int MinersWithProfit();
    void applyNegativeReputation(Miner* miner);
};

//--------------------------------------------------------------------------------

class Pools {
private:
    Pools(int population);
    ~Pools();
    core::list<PoolManager*> poolList;
    MinerPopulation* MP = &MinerPopulation::instance();
    PopulationParameters* populationP = &PopulationParameters::instance();
    core::Random* gen = &core::Random::instance();
    Stats* variableP = &Stats::instance();
    PoolManager* getPool (unsigned index);
    void makePools(int number);
    std::string poolFile = "Data/pools.db";
    bool readPools ();
    void writePools ();
    void updateVariableP();
public:
    static Pools& instance(int population=INT_MIN) {
        static Pools instance(population);
        return instance;
    }
    Pools(Pools &orig) = delete;
    Pools operator=(Pools &orig) = delete;
    PoolManager* operator [] (unsigned index);
    unsigned int size();
    void print();
    void shuffle();
    void saveIndex();
    void removeLostPools();
};

//--------------------------------------------------------------------------------

class Blockchain {
private:
    Blockchain();
    ~Blockchain();
    Blockchain(const Blockchain& orig) = delete;
};

//--------------------------------------------------------------------------------

#endif /* structures_hpp */
