#ifndef structures_hpp
#define structures_hpp

#include "BaseEntities.h"


class MinerPopulation {
private:
    MinerPopulation(int population);
    ~MinerPopulation();
    core::list<Miner*> list;
    core::list<Miner*> list_inactive;
    long totalHashPower_;
    int allViolationsCount = 0;
    int detectedViolationsCount = 0;
    std::string file;
    std::string file_inactive;
    int soloMiners = 0;
    void writeMinersAttr(core::list<Miner*> &L, std::ofstream &out);
    void readMinersAttr(core::list<Miner*> &L, std::ifstream &in);
    void writeMinerPopulation();
    bool readMinerPopulation();
    void writeMinerPopulation_inactive();
    bool readMinerPopulation_inactive();
    void addMiner();
    void deleteMiner(unsigned index);
    void shuffleValueGen();
    void saveOldOrder();
    void updateVariableParameters();
    PopulationParameters* populationP = &PopulationParameters::instance();
    core::Random* gen = &core::Random::instance();
    VariableParameters* variableP = &VariableParameters::instance();
    VirtualTime* T = &VirtualTime::instance();
    int removeLostMiners();
    void removeLosingMinersFromPools();
public:
    static MinerPopulation& instance(int population=INT_MIN) {
        static MinerPopulation instance(population);
        return instance;
    }
    MinerPopulation(MinerPopulation &orig) = delete;
    unsigned int size();
    void updateList();
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
    void printInactiveMiners();
    void printPopulationStat();
    void writeMinersInvitations(Pools* P);
    void readMinersInvitations(Pools* P);
    int topHashPower();
    int MinersWithAtLeastOneBlock();
    int MinersWithProfit();
};

//--------------------------------------------------------------------------------

class Pools {
private:
    Pools(int population);
    ~Pools();
    core::list<PoolManager*> poolList;
    MinerPopulation* MP = &MinerPopulation::instance(0);
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
