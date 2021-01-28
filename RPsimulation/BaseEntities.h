#ifndef BaseEntities_h
#define BaseEntities_h

#include "generators.h"
#include "listDS.h"

struct providedMiners {
    Miner* miner;
    double expectedDailyProfit = 0;
};

struct poolEvaluation {
    PoolManager* PM;
    Money EstimatedDailyProfit;
};

struct machine {
    char name[20];
    double hashRate;
    int wattage;
};

struct miningCycle {
    int rounds = 0;
    int powCount = 0;
    double expectedPowCount = 0;
};

//----------------------------------------------------------------------------------

class SineWaveModulator {
private:
    SineWaveModulator(double range, long minPeriod, long maxPeriod);
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
    std::vector<std::string> ModulatorNames;
    std::string file = "Data/modulators.db";
    void applyModulators();
    bool readFile();
    void writeFile();
    void CsvFileInit();
    void addModValuesToCsvFile();
public:
    static MasterTime& instance() {
        static MasterTime instance;
        return instance;
    }
    long getCurrentTime();
    MasterTime operator=(const MasterTime & orig) = delete;
    long addSecondsToCurrentTime(long seconds);
    long addHoursToCurrentTime(double hours);
    void createNewModulator(double range, long minPeriod, long maxPeriod, std::string key);
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
    Miner(char option='r');
    std::string firstName;
    std::string lastName;
    long idValue;
    std::time_t joinTimestamp;
    std::time_t quitTimestamp;
    bool rational;
    int joinedRound;
    int leftRound;
    double reputation;
    double expectedPowCount;
    double reputationTimeOffset; // If violation is detected a negative time offset is added which will reduce the reputation.
    int hashPower;
    int BWAttack;
    long violationTimeOffset;
    double incomePow;
    double incomePool;
    double incomeDishonesty;
    Money powerCostPerHour;
    Money powerConRate;
    Money costs;
    Money investment;
    Money targetProfit;
    Money lossTolerance;
    int roundsPlayed;
    double dishonestyFactor;
    bool taken;
    int powCount;
    Time minedTime;
    long poolJoined;
    long shuffleValue;
    long index;
    long oldIndex;
    bool BW_assigned;
    bool soloMinigNotProfitable = true;
    struct machine m;
    double probabilityConfidence;
    int receivedInvitationsCount;
    miningCycle currentPoolCycle;
    core::list<poolEvaluation> invitations;
    PoolManager* pool;
    VirtualTime* T = &VirtualTime::instance();
    core::Random* gen = &core::Random::instance();
    MiningParameters* miningP = &MiningParameters::instance();
    Stats* variableP = &Stats::instance();
    Money estimateSoloMiningProfit();
    Money estimatePoolProfit(PoolManager* PM, bool newPool=true);
    bool loseIfMineSolo(Money dailyCost, Money dailyProfit, Money reward);
    void initialize();
    void resetGlobalPars();
    void generateInitialValues();
    void powerCostCalculator();
    bool poolCommitmentTimeIsOver();
public:
    friend class MinerPopulation;
    friend class Game;
    friend class Reputation;
    friend class BW_Attack;
    friend bool compareID(Miner* a, Miner* b);
    friend bool compareDfact(Miner* a, Miner* b);
    friend bool compareMiningPower(Miner* a, Miner* b);
    friend bool compareJoinDate(Miner* a, Miner* b);
    friend bool compareMined(Miner* a, Miner* b);
    friend bool compareShuffleValue(Miner* a, Miner* b);
    friend bool compareOldIndex(Miner* a, Miner* b);
    friend bool compareBWAttack(Miner* a, Miner* b);
    friend bool compareProfit(Miner* a, Miner* b);
    friend bool compareRep(Miner* a, Miner* b);
    friend bool compareInvitationCount(Miner* a, Miner* b);
    bool newMiner;
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
    void incrementPlayedRound();
    miningCycle& getMiningCycle();
    double getExpectedMinedBlocks();
    void receivePoolRewards(double amount);
    void receivePowRewards(double amount);
    void receiveBribe(double amount);
    void addCost(Money amount);
    void receiveInvitation(PoolManager* p);
    bool isBellowLossTolerance();
    bool isReachedTargetProfit();
    void savePoolManager(PoolManager* poolManager);
    void removePoolManager(PoolManager* poolManager);
    void processInvitation();
    void selectTheMostProfitablePool(const std::vector<PoolManager*> & PM); // For normal (non-reputation) mode
    bool BribeIsAccepted(double bribePercentage, int numberOfRounds);
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
bool compareBWAttack(Miner* a, Miner* b);
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
bool compareProfit_p(providedMiners a, providedMiners b);
bool compareRep_p(providedMiners a, providedMiners b);
bool compareScore(providedMiners a, providedMiners b);

double accessRep(providedMiners a);
int accessMiningPower(providedMiners a);

bool compareIndex(PoolManager* a, PoolManager* b);
bool compareHash(PoolManager* a, PoolManager* b);

//--------------------------------------------------------------------------------

class MiningPool {
protected:
    friend bool compareMiningPower(Miner* a, Miner* b);
    core::list<Miner*> miners;
    std::string name;
    EntityParameters* entityP = &EntityParameters::instance();
    core::Random* gen = &core::Random::instance();
    void initialize();
    unsigned int totalHashPower;
    double poolFee;
    double powReward;
    MiningPool();
    MiningPool operator=(const MiningPool & orig) = delete;
    unsigned size();
    std::string& poolName();
    void distributeMinersReward(double amount, Miner* miner);
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
    double poolIncome;
    double poolDishonestIncome;
    Money cost;
    double bribePayed;
    Money costPerMiner;
    Money lossTolerance;
    int powCount;
    int dishonestPowCount;
    bool openToNewMiner;
    double expectedPowCount;
    int detectionCycle;
    bool dishonest;
    core::Random* gen = &core::Random::instance();
    long index;
    void initialize();
    void generate();
    void pushBack(Miner* miner);
    double aveRep();
    long minimumMembershipTime;
    void calculateCandidatesScore();
    double calculateProfitForNewMiner(Miner* miner);
    void resetMinersRoundCycle(Miner* miner);
    bool isBellowLossTolerance();
    void releaseAllMiners();
    bool BwAttackIsDetected();
    int sendInvitationsCount = 0;
    std::time_t establishedTime;
    core::list<providedMiners> candidateMinersList;
    MiningParameters* miningP = &MiningParameters::instance();
    EntityParameters* entityP = &EntityParameters::instance();
    Stats* variableP = &Stats::instance();
    BW_Attack_Data* BW = &BW_Attack_Data::instance();
    BwAttackParameters* BWP = &BwAttackParameters::instance();
public:
    friend class Pools;
    friend class Game;
    friend bool compareIndex(PoolManager* a, PoolManager* b);
    friend bool compareHash(PoolManager* a, PoolManager* b);
    PoolManager operator=(const PoolManager & orig) = delete;
    void print();
    unsigned size();
    int getPowCount();
    long getMinimumMembershipTime();
    Money getProfit();
    std::string getFirstName();
    std::string getLastName();
    long getID();
    std::time_t getEstablishedTime();
    std::string poolName();
    unsigned int poolHashPower();
    double poolFee();
    double poolPowReward();
    double getHashShareInPercentile();
    bool isDishonest();
    bool isOpenToNewMiners();
    long getIndex();
    Miner* getMiner(unsigned index);
    void updateCost(Time miningTime);
    void receiveReward(double amount, Miner* miner);
    void receiveDishonestReward(Miner* miner, double reward);
    bool pickMiner(Miner* miner);
    bool releaseMiner(Miner* miner);
    void receiveCandidateMiner(Miner* miner);
    void payBribe(Miner* miner, double minerBribe);
    Money estimateDailyRevenueForMiners(int hash=0);
    Money estimateDailyPowRewardForMiners(int hash);
    void receiveAcceptedInvitation(Miner* miner);
    void processCandidateMiners();
    void printPoolMiners();
    void printCandidateMiners();
    void clearMinersCandidateList();
    void sortMiners(std::string by="");
    void detectBwMiners(core::list<Miner*> & mList);
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


