#ifndef parameters_hpp
#define parameters_hpp

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>

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
    double powerConsumptionMean = 0.09;
    double powerConsumptionStd = 0.04;
    double powerConsumptionMin = 0.02;
    double powerConsumptionMax = 0.16;
    double poolFeeMin = 15;
    double poolFeeMax = 45;
    double POWrewardMax = 0.05;
    double POWrewardMin = 0.0;
    double POWrewardMean = 0.02;
    double POWrewardStd = 0.02;
    double poolSizeMean = 0.03;
    double poolSizeStd = 0.12;
    double poolSizeMin = 0.003;
    double poolSizeMax = 0.49;
    double probabilityConfidenceMean = 0.8;
    double probabilityConfidenceStd = 0.15;
    double probabilityConfidenceMin = 0.58;
    double probabilityConfidenceMax = 0.95;
    double lossToleranceFactorMin = 0.1;
    double lossToleranceFactorMax = 0.35;
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
    double getPoolSize(std::string parameter);
    double getProbabilityConfidence(std::string parameter);
    double lossToleranceFactor(std::string parameter);
};

//----------------------------------------------------------------------------------

class MiningParameters {
private:
    long zeroTimeOffset = 1000228870;
    int miningTimeMean = 600;                   // Average mining time for a New Block
    int miningTimeStd = 300;
    double minerMachinePricePerTh = 35;
    double revenueFactor = 0.3;
    double revenueFunctionSteepness = -0.0002;
    double revenueFunctionZeroPoint = 0.7;
    MiningParameters() {}
public:
    static MiningParameters& instance() {
        static MiningParameters instance;
        return instance;
    }
    MiningParameters operator=(const MiningParameters & orig) = delete;
    friend class Game;
    int getAverageMiningTime();
    double getMiningTime(std::string parameter);
    unsigned long getZeroTimeOffset();
    double getMinerMachinePricePerTh();
};

//----------------------------------------------------------------------------------

class PopulationParameters {
private:
    unsigned minersCarryingCapacity = 85256;
    long halfMinersCarryingCapacityTime = 32072000;
    double populationFunctionSteepness = 0.0000002;
    unsigned defaultNumberOfPool = 32;
    PopulationParameters() {}
public:
    static PopulationParameters& instance() {
        static PopulationParameters instance;
        return instance;
    }
    PopulationParameters operator=(const PopulationParameters & orig) = delete;
    friend double calculatePopulationGrowth();
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
};

//----------------------------------------------------------------------------------

class VariableParameters {
public:
    static VariableParameters& instance() {
        static VariableParameters instance;
        return instance;
    }
    VariableParameters operator=(const VariableParameters & orig) = delete;
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
    friend void printStats();
    friend void saveStats_csv();
    long getCurrentTotalHashPower();
    long getCurentMinersPopulation();
    long getCurrentPoolsPopulation();
    double getUnitPrice();
    double getUnitPerNewBlock();
    void updateNumberOfPoolMiners(int i);
private:
    VariableParameters() {}
    double unitPrice = 0;
    double unitPerNewBlock = 0;
    long currentTotalHashPower = 0;
    long currentMinersPopulation = 0;
    long currentInactiveMinersPopulation = 0;
    long currentPoolsPopulation = 0;
    double currentCostRewardRatio = 0;
    int numberOfPoolMiners = 0;
    int MinersWithAtLeastOneBlock = 0;
    int currentHighestMinerReputation = 0;
    int currentLowestMinerReputation = 0;
    int numberOfAllViolations = 0;
    int numberOfDetectedViolations = 0;
    double totalRevenue = 0;
    double totalCost = 0;
    int totalMinedBlocks = 0;
    long lastGeneratedBlockTime = 0;
};

//----------------------------------------------------------------------------------

#endif /* parameters_hpp */
