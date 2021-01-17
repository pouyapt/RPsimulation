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
    double powerConsumptionMean = 0.065; //cents for kilowatt per hour
    double powerConsumptionStd = 0.015;
    double powerConsumptionMin = 0.02;
    double powerConsumptionMax = 0.09;
    double poolFeeMean = 0.014;
    double poolFeeStd = 0.0005;
    double poolFeeMin = 0.013;
    double poolFeeMax = 0.015;
    double probabilityConfidenceMean = 0.8;
    double probabilityConfidenceStd = 0.15;
    double probabilityConfidenceMin = 0.58;
    double probabilityConfidenceMax = 0.95;
    double lossToleranceFactorMin = 0.2;
    double lossToleranceFactorMax = 0.4;
    double targetProfitFactorMin = 0.15;
    double targetProfitFactorMax = 0.6;
    double leavePoolBeforeLooseFactorMean = 0.8;
    double leavePoolBeforeLooseFactorStd = 0.1;
    double leavePoolBeforeLooseFactorMin = 0.6;
    double leavePoolBeforeLooseFactorMax = 0.98;
    int poolMinumumMembershipTimeMean = 6; //in months
    int poolMinumumMembershipTimeStd = 2;
    int poolMinumumMembershipTimeMin = 3;
    int poolMinumumMembershipTimeMax = 9;
    int poolStartingInvestment = 10000;
    double poolCostForOneMinerPerHourMean = 0.008;
    double poolCostForOneMinerPerHourStd = 0.002;
    double poolCostForOneMinerPerHourMin = 0.005;
    double poolCostForOneMinerPerHourMax = 0.01;
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
    double getPOWreward(std::string parameter);
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
    double revenueRangeFactor = 0.65;      // The max range of ratio between rewards and costs (sigmoid Function parameter)
    double revenueFunctionSteepness = 0;   // Steepness (sigmoid Function parameter)
    double revenueFunctionZeroPoint = 0.78;// This value is multiplied by Maximum Population
    double priceFluctuationRange = 0.4;   // The maximum range for the price modulater random sine fuction
    long priceFluctuationMinPhase = 55000; // In seconds
    long priceFluctuationMaxPhase = 250000;
    double revenueZeroPointRange = 0.2;
    long revenueZeroPointMinPhase = 800000;
    long revenueZeroPointMaxPhase = 2600000;
    std::string file = "Data/mining_parameters.db";
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
    unsigned maximumMiners = 20000;                     //Maximum Population (numerator of Sigmoid function)
    long halfMaximumMinersTime = 32100000;              // The required duration to reach the half of maximum population
    double startingPopulationToMaximumRatio = 0.002;    // population at time 0 divided by the maximum Population
    unsigned defaultNumberOfPool = 6;
    double growthRandomnessRange = 1.5;
    long growthRandomnessRangeMinPhase = 1209600;
    long growthRandomnessRangeMaxPhase = 2419200;
    double populationFunctionSteepness = 0;             // Calculated by the program
    double maxPopulationGrowth = 0;                     // Calculated by the program
    std::string file = "Data/population_parameters.db";
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
    
};

//----------------------------------------------------------------------------------

char* convertToDate_Time(long time);

#endif /* parameters_hpp */
