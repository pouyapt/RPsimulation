#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"
#include "listDS.h"

struct machine {
    std::string name;
    double hashRate;
    int wattage;
};

static class MinerMachines {
private:
    core::list<machine> mList;
    std::string machineFile;
    core::Random* gen = &core::Random::instance();
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
    long idValue;
    std::time_t joinedTimestamp;
    double reputation;
    int miningPower;
    int dMiningPower;
    int detectedViolations;
    int allViolations;
    Money powerCostPerHour;
    Money costs;
    Money poolIncome;
    Money powIncome;
    Money investment;
    int roundsPlayed;
    double dishonestyFactor;
    bool taken;
    int mined;
    Time minedTime;
    long shuffleValue;
    int index;
    int oldIndex;
    Money powerConRate;
    Money lossTolerance;
    struct machine m;
    double probabilityConfidence;
    void initialize();
    void generateInitialValues();
    void powerCostCalculator();
    PoolManager* pool;
    VirtualTime* virtualTime = &VirtualTime::instance();
    core::Random* gen = &core::Random::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    struct poolEvaluation {
        PoolManager* PM;
        Money EstimatedDailyProfit;
    };
    core::list<poolEvaluation> invitations;
    void processInvitation();
    Money estimatePoolProfit(PoolManager* PM);
    void acceptInvitation(PoolManager* p);
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
    long getID();
    long getJoinedTimestamp();
    double getReputation();
    int getMiningPower(std::string mode="real");
    Money getPowerCostPerHour();
    int getDetectedViolations();
    int roundPlayed();
    Money getProfit();
    bool isTaken();
    int getMined();
    int getIndex();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void receivePoolRewards(Money amount);
    void receivePowRewards(Money amount);
    void addCost(Money amount);
    void receiveInvitation(PoolManager* p);
    void print();
};

//--------------------------------------------------------------------------------

class MiningPool {
private:
    core::list<Miner*> miners;
    std::string name;
    core::Random* gen = &core::Random::instance();
    void initialize();
protected:
    unsigned int TotalhashPower;
    double poolFee;
    double powReward;
    double hashSizeProportion;
    Money grossIncome;
    MiningPool();
    MiningPool operator=(const MiningPool & orig) = delete;
    unsigned size();
    std::string& poolName();
    void distributeMinersReward(Money amount, Miner* miner);
    void addMiner(Miner* miner);
    bool removeMiner(Miner* miner);
    bool minerIsMember(Miner* miner);
    Miner* returnMiner(unsigned index);
    void pushBack(Miner* miner);
};

//--------------------------------------------------------------------------------

struct providedMiners {
    Miner* miner;
    bool invite=false;
};

class PoolManager: MiningPool {
private:
    std::string firstName;
    std::string lastName;
    long idValue;
    Money profit;
    int mined;
    int desiredHash;
    bool openToNewMiner;
    core::Random* gen = &core::Random::instance();
    void initialize();
    void generate();
    void computeDesiredHash();
    void pushBack(Miner* miner);
    std::time_t establishedTime;
    core::list<providedMiners> candidateMinersList;
    MiningParameters* miningP = &MiningParameters::instance();
    VariableParameters* variableP = &VariableParameters::instance();
public:
    PoolManager(std::string mode="default");
    PoolManager operator=(const PoolManager & orig) = delete;
    void print();
    unsigned size();
    int getMined();
    Money getProfit();
    std::string getFirstName();
    std::string getLastName();
    long getID();
    std::time_t getEstablishedTime();
    std::string poolName();
    unsigned int poolHashPower();
    double poolFee();
    Money poolRewards();
    bool isOpenToNewMiners();
    Miner* getMiner(unsigned index);
    void receiveReward(Money amount, Miner* miner);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    void receiveMinersList(core::list<providedMiners>& list);
    double getDailyPowProbability();
    friend class Pools;
};

//--------------------------------------------------------------------------------

class Block {
private:
    unsigned int blockID;
    double cryptoUnits;
    Miner* owner;
    unsigned int ownerID;
    std::time_t createdTime;
    Block* previous;
    core::list<Block*> next;
    bool verified;
};

//--------------------------------------------------------------------------------

#endif


