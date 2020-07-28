#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"
#include "listDS.h"

class PoolManager;
class MinerPopulation;

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
    double profit;
    int roundsPlayed;
    double dishonestyFactor;
    bool taken;
    int mined;
    int shuffleValue;
    int index;
    int oldIndex;
    bool endMining;
    double powerConRate;
    void initialize();
    void generateInitialValues();
    PoolManager* pool;
public:
    friend class MinerPopulation;
    friend void mineGame(MinerPopulation & DB);
    friend void updateMinersPowerCost(MinerPopulation & DB);
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
    int getDetectedViolations();
    int roundPlayed();
    double getProfit();
    bool isTaken();
    int getMined();
    bool miningIsEnded();
    int getIndex();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void receiveRewards(double amount);
    void reduceCost(double amount);
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
    long double grossIncome;
    MiningPool();
    MiningPool operator=(const MiningPool & orig) = delete;
    unsigned size();
    std::string& poolName();
    void distributeMinersReward(double amount);
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
    double profit;
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
    double getProfit();
    std::string getFirstName();
    std::string getLastName();
    unsigned getID();
    std::string poolName();
    unsigned int poolHashPower();
    double poolFee();
    double poolRewards();
    Miner* getMiner(unsigned index);
    void receiveReward(double amount);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    friend class Pools;
};

//--------------------------------------------------------------------------------

#endif


