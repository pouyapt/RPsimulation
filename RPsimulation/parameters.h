#ifndef parameters_hpp
#define parameters_hpp

#include "foundation.h"


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
    int minersPopulationGrowthMean;
    int minersPopulationGrowthStd;
    int minersPopulationGrowthMin;
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
    int getMinersPopulationGrowthPars(std::string parameter);
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
    double unitPrice;           // The Price of a Single Currency Unit
    double unitPerNewBlock;     // Number of Reward Units for a New Block
    int miningTimeMean;       // Average mining time for a New Block
    int miningTimeStd;
    double minerMachinePricePerTh;
    MiningParameters();
public:
    static MiningParameters& instance() {
        static MiningParameters instance;
        return instance;
    }
    int getAverageMiningTime();
    double getUnitPrice();
    double getUnitPerNewBlock();
    double getMiningTime(std::string parameter);
    unsigned long getCreationTime();
    double getMinerMachinePricePerTh();
};

//----------------------------------------------------------------------------------

class PopulationParameters {
private:
    unsigned defaultMinersPopulation;
    unsigned defaultNumberOfPool;
    PopulationParameters();
public:
    static PopulationParameters& instance() {
        static PopulationParameters instance;
        return instance;
    }
    friend class MinerPopulation;
    friend class Pools;
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
    long getCurrentTotalHashPower();
    long getCurentMinersPopulation();
    long getCurrentPoolsPopulation();
private:
    VariableParameters() {}
    long currentTotalHashPower = 0;
    long currentMinersPopulation = 0;
    long currentPoolsPopulation = 0;
};

//----------------------------------------------------------------------------------

#endif /* parameters_hpp */
