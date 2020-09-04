#ifndef parameters_hpp
#define parameters_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>

class EntityParameters {
private:
    EntityParameters();
    int miningPowerMean;
    int miningPowerStd;
    int miningPowerMin;
    int miningPowerLowRange;
    int dishonestyFactorMean;
    int dishonestyFactorStd;
    int dishonestyFactorMin;
    double powerConsumptionMean;
    double powerConsumptionStd;
    double powerConsumptionMin;
    double powerConsumptionMax;
    double poolFeeMin;
    double poolFeeMax;
    double POWrewardMean;
    double POWrewardStd;
    double POWrewardMin;
    double POWrewardMax;
    double poolSizeMean;
    double poolSizeStd;
    double poolSizeMin;
    double poolSizeMax;
    double probabilityConfidenceMean;
    double probabilityConfidenceStd;
    double probabilityConfidenceMin;
    double probabilityConfidenceMax;
    double lossToleranceFactorMin;
    double lossToleranceFactorMax;
public:
    static EntityParameters& instance() {
        static EntityParameters instance;
        return instance;
    }
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
    unsigned long creationTime;
    int miningTimeMean;       // Average mining time for a New Block
    int miningTimeStd;
    double minerMachinePricePerTh;
    MiningParameters();
public:
    static MiningParameters& instance() {
        static MiningParameters instance;
        return instance;
    }
    friend class Game;
    int getAverageMiningTime();
    double getMiningTime(std::string parameter);
    unsigned long getCreationTime();
    double getMinerMachinePricePerTh();
};

//----------------------------------------------------------------------------------

class PopulationParameters {
private:
    unsigned minersCarryingCapacity;
    double minersPopulationGrowthRate;
    unsigned zeroRevenuePopulation;
    unsigned defaultNumberOfPool;
    PopulationParameters();
public:
    static PopulationParameters& instance() {
        static PopulationParameters instance;
        return instance;
    }
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
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
    long getCurrentTotalHashPower();
    long getCurentMinersPopulation();
    long getCurrentPoolsPopulation();
    double getUnitPrice();
    double getUnitPerNewBlock();
    friend void printStats();
private:
    VariableParameters() {}
    double unitPrice = 0;
    double unitPerNewBlock = 0;
    long currentTotalHashPower = 0;
    long currentMinersPopulation = 0;
    long currentInactiveMinersPopulation = 0;
    long currentPoolsPopulation = 0;
    double currentCostRewardRatio = 0;
    int numberOfSoloMiners = 0;
    int numberOfPoolMiners = 0;
    int MinersWithAtLeastOneBlock = 0;
    int currentHighestMinerReputation = 0;
    int currentLowestMinerReputation = 0;
    int numberOfAllViolations = 0;
    int numberOfDetectedViolations = 0;
    double totalRevenue = 0;
    double totalCost = 0;
    int totalMinedBlocks = 0;
};

//----------------------------------------------------------------------------------

#endif /* parameters_hpp */
