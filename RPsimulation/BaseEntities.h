#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"


struct machine {
    std::string name;
    double hashRate;
    int wattage;
};

static class MinerMachines {
private:
    core::list<machine> mList;
    std::string machineFile;
    bool readFile();
public:
    MinerMachines();
    machine randomPick();
} machines;

//----------------------------------------------------------------------------------

class Miner {
private:
    std::string firstName;
    std::string lastName;
    unsigned int idValue;
    std::time_t joinedTimestamp;
    double reputation;
    int miningPower;
    int dMiningPower;
    int detectedViolations;
    int allViolations;
    Money powerCostPerHour;
    Money costs;
    Money income;
    int roundsPlayed;
    double dishonestyFactor;
    bool taken;
    int mined;
    int shuffleValue;
    int index;
    int oldIndex;
    bool endMining;
    Money powerConRate;
    struct machine m;
    void initialize();
    void generateInitialValues();
    void powerCostCalculator();
    PoolManager* pool;
public:
    friend class MinerPopulation;
    friend class Game;
    friend bool compareID(Miner* a, Miner* b);
    friend bool compareDfact(Miner* a, Miner* b);
    friend bool compareMiningPower(Miner* a, Miner* b);
    friend bool compareJoinDate(Miner* a, Miner* b);
    friend bool compareMined(Miner* a, Miner* b);
    friend bool compareShuffleValue(Miner* a, Miner* b);
    friend bool compareOldIndex(Miner* a, Miner* b);
    friend bool compareDViolation(Miner* a, Miner* b);
    friend bool compareAViolation(Miner* a, Miner* b);
    friend bool compareProfit(Miner* a, Miner* b);
    Miner(std::string option=" ");
    Miner operator=(const Miner & orig) = delete;
    std::string getFirstName();
    std::string getLastName();
    unsigned int getID();
    long getJoinedTimestamp();
    double getReputation();
    int getMiningPower(std::string mode="real");
    Money getPowerCostPerHour();
    int getDetectedViolations();
    int roundPlayed();
    Money getProfit();
    bool isTaken();
    int getMined();
    bool miningIsEnded();
    int getIndex();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void receiveRewards(Money amount);
    void addCost(Money amount);
    void print();
};

//--------------------------------------------------------------------------------

class MiningPool {
private:
    core::list<Miner*> miners;
    std::string name;
    void initialize();
protected:
    unsigned int TotalhashPower;
    double poolFee;
    Money grossIncome;
    MiningPool();
    MiningPool operator=(const MiningPool & orig) = delete;
    unsigned size();
    std::string& poolName();
    void distributeMinersReward(Money amount);
    void addMiner(Miner* miner);
    bool removeMiner(Miner* miner);
    bool minerIsMember(Miner* miner);
    Miner* returnMiner(unsigned index);
    void pushBack(Miner* miner);
};

//--------------------------------------------------------------------------------

class PoolManager: MiningPool {
private:
    std::string firstName;
    std::string lastName;
    unsigned idValue;
    Money profit;
    int mined;
    void initialize();
    void generate();
    void pushBack(Miner* miner);
public:
    PoolManager(std::string mode="default");
    PoolManager operator=(const PoolManager & orig) = delete;
    void print();
    unsigned size();
    int getMined();
    Money getProfit();
    std::string getFirstName();
    std::string getLastName();
    unsigned getID();
    std::string poolName();
    unsigned int poolHashPower();
    double poolFee();
    Money poolRewards();
    Miner* getMiner(unsigned index);
    void receiveReward(Money amount);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    friend class Pools;
};

//--------------------------------------------------------------------------------


#endif


