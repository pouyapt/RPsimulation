#ifndef parameters_hpp
#define parameters_hpp

#include "BasicFunctions.h"

class EntityParameters {
private:
    EntityParameters() {}
    int miningPowerMean = 82;
    int miningPowerStd = 6976;
    int miningPowerMin = 8;
    int miningPowerLowRange = 72;
    int dishonestyFactorMean = 0;
    int dishonestyFactorStd = 10;
    int dishonestyFactorMin = 0;
    double powerConsumptionMean = 0.08;
    double powerConsumptionStd = 0.04;
    double powerConsumptionMin = 0.02;
    double powerConsumptionMax = 0.13;
    double poolFeeMin = 15;
    double poolFeeMax = 45;
    double POWrewardMax = 0.05;
    double POWrewardMin = 0.0;
    double POWrewardMean = 0.02;
    double POWrewardStd = 0.02;
    double probabilityConfidenceMean = 0.8;
    double probabilityConfidenceStd = 0.15;
    double probabilityConfidenceMin = 0.58;
    double probabilityConfidenceMax = 0.95;
    double lossToleranceFactorMin = 0.1;
    double lossToleranceFactorMax = 0.35;
    double leavePoolBeforeLooseFactorMean = 0.8;
    double leavePoolBeforeLooseFactorStd = 0.1;
    double leavePoolBeforeLooseFactorMin = 0.6;
    double leavePoolBeforeLooseFactorMax = 0.98;
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
    double leavePoolBeforeLooseFactor(std::string parameter);
};

//----------------------------------------------------------------------------------

class MiningParameters {
private:
    MiningParameters();
    ~MiningParameters();
    long zeroTimeOffset = 1042228870;       // Doesn't affect the program's internal time. Used only to show the time to the user.
    int miningTimeMean = 600;               // Average mining time for a New Block
    int miningTimeStd = 300;                // Standard Diviation for time for a New Block
    double minerMachinePricePerTh = 35;
    double revenueRangeFactor = 0.4;        // The max range of ratio between rewards and costs (sigmoid Function parameter)
    double revenueFunctionSteepness = 0;    // Steepness (sigmoid Function parameter)
    double revenueFunctionZeroPoint = 0.78; // This value is multiplied by Maximum Population
    double priceFluctuationFactor = 0.1;
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
    unsigned maximumMiners = 86000;                     //Maximum Population (numerator of Sigmoid function)
    long halfMaximumMinersTime = 32100000;              // The required duration to reach the half of maximum population
    double startingPopulationToMaximumRatio = 0.002;    // population at time 0 divided by the maximum Population
    unsigned defaultNumberOfPool = 5;
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

char* convertToDate_Time(long time);

#endif /* parameters_hpp */
