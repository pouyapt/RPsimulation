#ifndef structures_hpp
#define structures_hpp

#include "BaseEntities.h"


class MinerPopulation {
private:
    MinerPopulation();
    ~MinerPopulation();
    core::list<Miner*> mainList;
    core::list<Miner*> removedList;
    long totalHashPower_;
    int allViolationsCount = 0;
    int detectedViolationsCount = 0;
    long removedMinersCount = 0;
    std::string populationDataFile = "Data/p_data.db";
    std::string minersDataFile = "Data/p_list.db";
    std::string removedMinersDataFile = "Data/p_list_removed.db";
    void writeMinersData(std::ofstream &out);
    void readMinersData(std::ifstream &in, int size);
    void writePopulationData();
    bool readPopulationData();
    void writeRemovedMiners();
    void addMiner(long time=0);
    void deleteMiner(unsigned index);
    void shuffleValueGen();
    void saveOldOrder();
    void updateVariableParameters();
    PopulationParameters* populationP = &PopulationParameters::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    core::Random* gen = &core::Random::instance();
    VariableParameters* variableP = &VariableParameters::instance();
    MasterTime* T = &MasterTime::instance();
    void removeLostMiners();
    void removeLosingMinersFromPools();
    int calculateNewPopulation();
public:
    static MinerPopulation& instance() {
        static MinerPopulation instance;
        return instance;
    }
    MinerPopulation(MinerPopulation &orig) = delete;
    unsigned int size();
    void updateLosingMiners();
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
    void updatePopulation(long time);
    int topHashPower();
    int MinersWithAtLeastOneBlock();
    int MinersWithProfit();
    double population;
};

//--------------------------------------------------------------------------------

class Pools {
private:
    Pools(int population);
    ~Pools();
    core::list<PoolManager*> poolList;
    MinerPopulation* MP = &MinerPopulation::instance();
    PopulationParameters* populationP = &PopulationParameters::instance();
    VariableParameters* variableP = &VariableParameters::instance();
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
    void saveIndex();
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
