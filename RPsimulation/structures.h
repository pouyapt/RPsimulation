#ifndef structures_hpp
#define structures_hpp

#include "BaseEntities.h"

#define DF 46346        // Default/Ma*ximum Miner Population
#define DP 24


bool compareID(Miner* a, Miner* b);
bool compareDfact(Miner* a, Miner* b);
bool compareMiningPower(Miner* a, Miner* b);
bool compareJoinDate(Miner* a, Miner* b);
bool compareMined(Miner* a, Miner* b);
bool compareShuffleValue(Miner* a, Miner* b);
bool compareOldIndex(Miner* a, Miner* b);
bool compareDViolation(Miner* a, Miner* b);
bool compareAViolation(Miner* a, Miner* b);

//--------------------------------------------------------------------------------

class MinerPopulation {
private:
    core::list<Miner*> list;
    core::list<Miner*> list_inactive;
    unsigned int totalHashPower_;
    int allViolationsCount = 0;
    int detectedViolationsCount = 0;
    std::string file;
    void writeMinerPopulation ();
    bool readMinerPopulation ();
    void addMiner();
    void deleteMiner(unsigned index);
    void shuffleValueGen();
    void saveOldOrder();
public:
    MinerPopulation(int population=DF, std::string file="Data/miners.db");
    ~MinerPopulation();
    MinerPopulation(MinerPopulation &orig) = delete;
    unsigned int size();
    void updateList();
    Miner* operator [] (unsigned index);
    unsigned totalHashPower();
    int getAllViolationsCount();
    int& getDetectedViolationsCount();
    void saveIndex();
    void sort(std::string by="");
    void print();
};

//--------------------------------------------------------------------------------

class Pools {
private:
    core::list<PoolManager*> poolList;
    MinerPopulation* R;
    PoolManager* getPool (unsigned index);
    void makePools(int number);
    std::string poolFile = "Data/pools.db";
    bool readPools (MinerPopulation & DB);
    void writePools (MinerPopulation & DB);
public:
    Pools(MinerPopulation & DB);
    Pools(Pools &orig) = delete;
    PoolManager* operator [] (unsigned index);
    ~Pools();
    unsigned int size();
    void print();
    
};

//--------------------------------------------------------------------------------

#endif /* structures_hpp */
