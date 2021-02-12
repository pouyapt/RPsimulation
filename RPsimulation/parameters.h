#ifndef parameters_hpp
#define parameters_hpp

#include "BasicFunctions.h"

class EntityParameters {
private:
    EntityParameters() {}
    int miningPowerMean = 82; //Th per second
    int miningPowerStd = 6976;
    int miningPowerMin = 8;
    int miningPowerLowRange = 72;
    int dishonestyFactorMean = 5;
    int dishonestyFactorStd = 5;
    int dishonestyFactorMin = 0;
    double powerConsumptionMean = 0.06; //cents for kilowatt per hour
    double powerConsumptionStd = 0.015;
    double powerConsumptionMin = 0.04;
    double powerConsumptionMax = 0.08;
    double poolFeeMean = 0.014;
    double poolFeeStd = 0.0005;
    double poolFeeMin = 0.013;
    double poolFeeMax = 0.015;
    double poolPowRewardMean = 0.03;
    double poolPowRewardStd = 0.02;
    double poolPowRewardMin = 0.02;
    double poolPowRewardMax = 0.04;
    double probabilityConfidenceMean = 0.8;
    double probabilityConfidenceStd = 0.15;
    double probabilityConfidenceMin = 0.58;
    double probabilityConfidenceMax = 0.95;
    double lossToleranceFactorMin = 0.5;
    double lossToleranceFactorMax = 1.5;
    double targetProfitFactorMin = 2.5;
    double targetProfitFactorMax = 6.5;
    double leavePoolBeforeLooseFactorMean = 0.8;
    double leavePoolBeforeLooseFactorStd = 0.1;
    double leavePoolBeforeLooseFactorMin = 0.6;
    double leavePoolBeforeLooseFactorMax = 0.98;
    int poolMinumumMembershipTimeMean = 6; //in months
    int poolMinumumMembershipTimeStd = 2;
    int poolMinumumMembershipTimeMin = 3;
    int poolMinumumMembershipTimeMax = 9;
    int poolStartingInvestment = 500000;
    double poolCostForOneMinerPerHourMean = 0.065;
    double poolCostForOneMinerPerHourStd = 0.02;
    double poolCostForOneMinerPerHourMin = 0.045;
    double poolCostForOneMinerPerHourMax = 0.085;
public:
    static EntityParameters& instance() {
        static EntityParameters instance;
        return instance;
    }
    EntityParameters operator=(const EntityParameters & orig) = delete;
    int getMiningPowerPars(std::string parameter);
    int getDishonestyFactorPars(std::string parameter);
    double getPowerConsumptionPars(std::string parameter);
    double getPoolFeesPars(std::string parameter);
    double getPoolPowRewardPars(std::string parameter);
    double getProbabilityConfidence(std::string parameter);
    double lossToleranceFactor(std::string parameter);
    double targetProfitFactor(std::string parameter);
    double leavePoolBeforeLooseFactor(std::string parameter);
    int poolMinumumMembershipTime(std::string parameter);
    int getPoolStartingInvestment();
    double poolCostForOneMinerPerHour(std::string parameter);
};

//----------------------------------------------------------------------------------

class MiningParameters {
private:
    MiningParameters();
    ~MiningParameters();
    long zeroTimeOffset = 1042228870; // Doesn't affect the program's internal time. Used only as an offset for printing.
    int miningTimeMean = 600;         // Average mining time for a New Block
    int miningTimeStd = 300;          // Standard Diviation for time for a New Block
    double minerMachinePricePerHash = 35;     // Average price for a mining machine with 1Th/s hash power
    double revenueRangeFactor = 0.35;      // The max range of ratio between rewards and costs (sigmoid Function parameter)
    double revenueFunctionSteepness = 0;   // Steepness (sigmoid Function parameter)
    double revenueFunctionZeroPoint = 0.78;// This value is multiplied by Maximum Population
    double priceFluctuationRange = 0.4;   // The maximum range for the price modulater random sine fuction
    long priceFluctuationMinPhase = 55000; // In seconds
    long priceFluctuationMaxPhase = 250000;
    double revenueZeroPointRange = 0.2;
    long revenueZeroPointMinPhase = 800000;
    long revenueZeroPointMaxPhase = 2600000;
    std::string file = "data/mining_parameters.db";
    double calculateSteepness();
    bool readFiles();
    void writeFiles();
public:
    static MiningParameters& instance() {
        static MiningParameters instance;
        return instance;
    }
    MiningParameters operator=(const MiningParameters & orig) = delete;
    friend class Game;
    friend class MinerPopulation;
    int getAverageMiningTime();
    double getMiningTime(std::string parameter);
    unsigned long getZeroTimeOffset();
    double getMinerMachinePricePerTh();
};

//----------------------------------------------------------------------------------

class PopulationParameters {
private:
    unsigned maximumMiners = 30000;                     //Maximum Population (numerator of Sigmoid function)
    long halfMaximumMinersTime = 32100000;              // The required duration to reach the half of maximum population
    double startingPopulationToMaximumRatio = 0.002;    // population at time 0 divided by the maximum Population
    unsigned defaultNumberOfPool = 8;
    double growthRandomnessRange = 1.5;
    long growthRandomnessRangeMinPhase = 1209600;
    long growthRandomnessRangeMaxPhase = 2419200;
    double populationFunctionSteepness = 0;             // Calculated by the program
    double maxPopulationGrowth = 0;                     // Calculated by the program
    std::string file = "data/population_parameters.db";
    PopulationParameters();
    ~PopulationParameters();
    double calculateSteepness();
    bool readFiles();
    void writeFiles();
public:
    static PopulationParameters& instance() {
        static PopulationParameters instance;
        return instance;
    }
    PopulationParameters operator=(const PopulationParameters & orig) = delete;
    friend class MiningParameters;
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
};

//----------------------------------------------------------------------------------

class BwAttackParameters {
public:
    static BwAttackParameters& instance() {
        static BwAttackParameters instance;
        return instance;
    }
    friend class BW_Attack;
    friend class PoolManager;
private:
    BwAttackParameters() {}
    int minimumVictimHashShare = 3; // Minimum Victim Pool Hash Share in Percentile
    int victimPoolAttempts = 30; // Number of attempts to find corrupt miner from the victim pool
    int maxAttackCounts = 25; // Maximum number of attacks for each attack setup
    double maxMinerHashShare = 0.2; // Maximum miner's hash share in the victim pool to be assignable for the attack
    double minerBribeMin = 20; // Minimum miner's bribe in percentage of reward
    double minerBribeMax = 35; // Maximum miner's bribe in percentage of reward
};

//----------------------------------------------------------------------------------

class TrustAParameters {
public:
    static TrustAParameters& instance() {
        static TrustAParameters instance;
        return instance;
    }
private:
    friend class TrustA;
    friend class PoolManager;
    TrustAParameters() {}
    ~TrustAParameters() {}
    double alpha = 0.5;
    double beta = 0;
    double epsilon = 0.1;
    double theta = 0.05;
    double eta = 0.01;
    double kappa = 0.09;
    
};

//----------------------------------------------------------------------------------

class TrustBParameters {
public:
    static TrustBParameters& instance() {
        static TrustBParameters instance;
        return instance;
    }
private:
    friend class TrustB;
    friend class PoolManager;
    TrustBParameters() {}
    ~TrustBParameters() {}
    double steepness = 0.6;
    double log_(double x) { return log2(x); }
};

//----------------------------------------------------------------------------------

class ReputationParameters {
public:
    static ReputationParameters& instance() {
        static ReputationParameters instance;
        return instance;
    }
private:
    friend class PoolManager;
    double badMinerLowBound = -0.2;
    double goodMinerLowBound = 0.5;
    double badMinersP = 0.1;
    double newMinersP = 0.3;
    double goodMinersP = 0.6;
    double invitationPercentage = 0.25;
    int invitationProcessCycleMin = 3000;
    int invitationProcessCycleMax = 7000;
    int attackDetectionConfidence = 98; // The percentage of confidence interval for attack detection by the pool manager
    int attackDetectionCycleMin = 12000; // The number of rounds for detection cycle
    int attackDetectionCycleMax = 15000;
};

char* convertToDate_Time(long time);

#endif /* parameters_hpp */
