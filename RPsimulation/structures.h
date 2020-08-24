#ifndef structures_hpp
#define structures_hpp

#include "BaseEntities.h"


bool compareID(Miner* a, Miner* b);
bool compareDfact(Miner* a, Miner* b);
bool compareMiningPower(Miner* a, Miner* b);
bool compareJoinDate(Miner* a, Miner* b);
bool compareMined(Miner* a, Miner* b);
bool compareShuffleValue(Miner* a, Miner* b);
bool compareOldIndex(Miner* a, Miner* b);
bool compareDViolation(Miner* a, Miner* b);
bool compareAViolation(Miner* a, Miner* b);
bool compareProfit(Miner* a, Miner* b);

//--------------------------------------------------------------------------------

class MinerPopulation {
private:
    core::list<Miner*> list;
    core::list<Miner*> list_inactive;
    long totalHashPower_;
    int allViolationsCount = 0;
    int detectedViolationsCount = 0;
    std::string file;
    void writeMinerPopulation ();
    bool readMinerPopulation ();
    void addMiner();
    void deleteMiner(unsigned index);
    void shuffleValueGen();
    void saveOldOrder();
    void updateVariableParameters();
    PopulationParameters* populationP = &PopulationParameters::instance();
    core::Random* gen = &core::Random::instance();
    VariableParameters* variableP = &VariableParameters::instance();
    MinerPopulation(int population);
    ~MinerPopulation();
public:
    static MinerPopulation& instance(int population) {
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
    void print();
};

//--------------------------------------------------------------------------------

class Pools {
private:
    Pools();
    ~Pools();
    core::list<PoolManager*> poolList;
    MinerPopulation* MP = &MinerPopulation::instance(0);
    PoolManager* getPool (unsigned index);
    PopulationParameters* populationP = &PopulationParameters::instance();
    void makePools(int number);
    std::string poolFile = "Data/pools.db";
    bool readPools (MinerPopulation & DB);
    void writePools (MinerPopulation & DB);
public:
    static Pools& instance() {
        static Pools instance;
        return instance;
    }
    Pools(Pools &orig) = delete;
    PoolManager* operator [] (unsigned index);
    unsigned int size();
    void print();
    
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
