#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"
#include "listDS.h"

struct providedMiners {
    Miner* miner;
    bool invite=false;
    bool accept=false;
    double score=0;
};

struct poolEvaluation {
    PoolManager* PM;
    Money EstimatedDailyProfit;
};

struct machine {
    std::string name;
    double hashRate;
    int wattage;
};

//----------------------------------------------------------------------------------

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
    Miner(std::string option=" ");
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
    long index;
    long oldIndex;
    Money powerConRate;
    Money lossTolerance;
    struct machine m;
    double probabilityConfidence;
    int receivedInvitations;
    void initialize();
    void generateInitialValues();
    void powerCostCalculator();
    PoolManager* pool;
    VirtualTime* virtualTime = &VirtualTime::instance();
    core::Random* gen = &core::Random::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    core::list<poolEvaluation> invitations;
    void processInvitation();
    Money estimatePoolProfit(PoolManager* PM);
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
    friend bool compareRep(Miner* a, Miner* b);
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
    long getIndex();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void receivePoolRewards(Money amount);
    void receivePowRewards(Money amount);
    void addCost(Money amount);
    void receiveInvitation(PoolManager* p);
    bool isBellowLossTolerance();
    bool needsToExitPool();
    void print();
};

//--------------------------------------------------------------------------------

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
bool compareRep(Miner* a, Miner* b);

typedef bool (*compareFunc)(Miner* a, Miner* b);
compareFunc selectCompareFunc(std::string by);

bool compareID_p(providedMiners a, providedMiners b);
bool compareDfact_p(providedMiners a, providedMiners b);
bool compareMiningPower_p(providedMiners a, providedMiners b);
bool compareJoinDate_p(providedMiners a, providedMiners b);
bool compareMined_p(providedMiners a, providedMiners b);
bool compareShuffleValue_p(providedMiners a, providedMiners b);
bool compareOldIndex_p(providedMiners a, providedMiners b);
bool compareDViolation_p(providedMiners a, providedMiners b);
bool compareAViolation_p(providedMiners a, providedMiners b);
bool compareProfit_p(providedMiners a, providedMiners b);
bool compareRep_p(providedMiners a, providedMiners b);
bool compareScore(providedMiners a, providedMiners b);

double accessRep(providedMiners a);
int accessMiningPower(providedMiners a);

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

class PoolManager: MiningPool {
private:
    PoolManager(std::string mode="default");
    std::string firstName;
    std::string lastName;
    long idValue;
    Money profit;
    int mined;
    int desiredHash;
    bool openToNewMiner;
    core::Random* gen = &core::Random::instance();
    int index;
    void initialize();
    void generate();
    double poolHashPercentage();
    void computeDesiredHash();
    void pushBack(Miner* miner);
    double aveRep();
    void calculateCandidatesScore();
    std::time_t establishedTime;
    core::list<providedMiners> candidateMinersList;
    MiningParameters* miningP = &MiningParameters::instance();
    VariableParameters* variableP = &VariableParameters::instance();
public:
    friend class Pools;
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
    int getIndex();
    Miner* getMiner(unsigned index);
    void receiveReward(Money amount, Miner* miner);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    void receiveMinersList(core::list<providedMiners>& list);
    double getDailyPowProbability();
    void receiveAcceptedInvitation(Miner* miner);
    void processCandidateMiners();
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


