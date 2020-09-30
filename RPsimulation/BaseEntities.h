#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"
#include "listDS.h"

struct providedMiners {
    Miner* miner;
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

class SineWaveModulator {
private:
    SineWaveModulator(double range, int minPeriod, int maxPeriod);
    SineWaveModulator() {}
    VirtualTime* V = &VirtualTime::instance();
    core::Random* gen = &core::Random::instance();
    double range_;
    int d;
    double maxR;
    double minP;
    double maxP;
    double l;
    double r;
    double a;
    double b;
    double x_e;
    double y_e;
    void generateParameters();
    double mainFunction(long t);
    double getValue();
public:
    friend class MasterTime;
};

//----------------------------------------------------------------------------------

class MasterTime {
private:
    MasterTime();
    ~MasterTime();
    VirtualTime* VT = &VirtualTime::instance();
    std::map<std::string, SineWaveModulator*> M;
    std::string file = "Data/modulators.db";
    void applyModulators();
    bool readFile();
    void writeFile();
public:
    static MasterTime& instance() {
        static MasterTime instance;
        return instance;
    }
    long getCurrentTime();
    MasterTime operator=(const MasterTime & orig) = delete;
    long addSecondsToCurrentTime(long seconds);
    long addHoursToCurrentTime(double hours);
    void createNewModulator(double range, int minPeriod, int maxPeriod, std::string key);
    double getModulatorValue(std::string key);
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
    int hashPower;
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
    int receivedInvitationsCount;
    void initialize();
    void generateInitialValues();
    void powerCostCalculator();
    PoolManager* pool;
    MasterTime* T = &MasterTime::instance();
    core::Random* gen = &core::Random::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    Stats* variableP = &Stats::instance();
    core::list<poolEvaluation> invitations;
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
    friend bool compareInvitationCount(Miner* a, Miner* b);
    Miner operator=(const Miner & orig) = delete;
    std::string getFirstName();
    std::string getLastName();
    long getID();
    long getJoinedTimestamp();
    double getReputation();
    int getHashPower();
    Money getPowerCostPerHour();
    int getDetectedViolations();
    int roundPlayed();
    Money getProfit();
    bool isInPool();
    int getMined();
    long getIndex();
    bool extracted();
    void receivePoolRewards(Money amount);
    void receivePowRewards(Money amount);
    void addCost(Money amount);
    void receiveInvitation(PoolManager* p);
    bool isBellowLossTolerance();
    bool needsToExitPool();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void processInvitation();
    void print();
    void printInvitations();
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
bool compareInvitationCount(Miner* a, Miner* b);

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

bool compareIndex(PoolManager* a, PoolManager* b);
bool compareHash(PoolManager* a, PoolManager* b);

//--------------------------------------------------------------------------------

class MiningPool {
private:
    core::list<Miner*> miners;
    std::string name;
    core::Random* gen = &core::Random::instance();
    void initialize();
protected:
    unsigned int totalHashPower;
    double poolFee;
    double powReward;
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
    bool openToNewMiner;
    core::Random* gen = &core::Random::instance();
    long index;
    void initialize();
    void generate();
    void pushBack(Miner* miner);
    double aveRep();
    void calculateCandidatesScore();
    int sendInvitationsCount = 0;
    std::time_t establishedTime;
    core::list<providedMiners> candidateMinersList;
    MiningParameters* miningP = &MiningParameters::instance();
    Stats* variableP = &Stats::instance();
public:
    friend class Pools;
    friend bool compareIndex(PoolManager* a, PoolManager* b);
    friend bool compareHash(PoolManager* a, PoolManager* b);
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
    long getIndex();
    Miner* getMiner(unsigned index);
    void receiveReward(Money amount, Miner* miner);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    void receiveCandidateMiner(Miner* miner);
    Money estimateDailyRevenue(int hash=0);
    void receiveAcceptedInvitation(Miner* miner);
    void processCandidateMiners();
    void printPoolMiners();
    void printCandidateMiners();
    void clearMinersCandidateList();
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


