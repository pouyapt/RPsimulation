#include "parameters.h"


EntityParameters::EntityParameters() {
    miningPowerMean = 82;
    miningPowerStd = 6976;
    miningPowerMin = 8;
    miningPowerLowRange = 72;
    dishonestyFactorMean = 0;
    dishonestyFactorStd = 10;
    dishonestyFactorMin = 0;
    powerConsumptionMean = 0.09;
    powerConsumptionStd = 0.04;
    powerConsumptionMin = 0.02;
    powerConsumptionMax = 0.16;
    poolFeeMin = 15;
    poolFeeMax = 45;
    POWrewardMax = 0.05;
    POWrewardMin = 0.0;
    POWrewardMean = 0.02;
    POWrewardStd = 0.02;
    poolSizeMean = 0.03;
    poolSizeStd = 0.12;
    poolSizeMin = 0.003;
    poolSizeMax = 0.49;
    probabilityConfidenceMean = 0.8;
    probabilityConfidenceStd = 0.15;
    probabilityConfidenceMin = 0.58;
    probabilityConfidenceMax = 0.95;
    lossToleranceFactorMin = 0.04;
    lossToleranceFactorMax = 0.18;
}

int EntityParameters::getMiningPowerPars(std::string parameter) {
    if (parameter=="mean")
        return miningPowerMean;
    else if (parameter=="std")
        return miningPowerStd;
    else if (parameter=="min")
        return miningPowerMin;
    else
        return miningPowerLowRange;
}

int EntityParameters::getDishonestyFactorPars(std::string parameter) {
    if (parameter=="mean")
        return dishonestyFactorMean;
    else if (parameter=="std")
        return dishonestyFactorStd;
    else
        return dishonestyFactorMin;
}

double EntityParameters::getPowerConsumptionPars(std::string parameter) {
    if (parameter=="mean")
        return powerConsumptionMean;
    else if (parameter=="std")
        return powerConsumptionStd;
    else if (parameter=="min")
        return powerConsumptionMin;
    else
        return powerConsumptionMax;
}

double EntityParameters::getPoolFeesPars(std::string parameter) {
    if (parameter=="min")
        return poolFeeMin;
    else
        return poolFeeMax;
}

double EntityParameters::getPOWreward(std::string parameter) {
    if (parameter=="mean")
        return POWrewardMean;
    else if (parameter=="std")
        return POWrewardStd;
    else if (parameter=="min")
        return POWrewardMin;
    else
        return POWrewardMax;
}

double EntityParameters::getPoolSize(std::string parameter) {
    if (parameter=="mean")
        return poolSizeMean;
    else if (parameter=="std")
        return poolSizeStd;
    else if (parameter=="min")
        return poolSizeMin;
    else
        return poolSizeMax;
}

double EntityParameters::getProbabilityConfidence(std::string parameter) {
    if (parameter=="mean")
        return probabilityConfidenceMean;
    else if (parameter=="std")
        return probabilityConfidenceStd;
    else if (parameter=="min")
        return probabilityConfidenceMin;
    else
        return probabilityConfidenceMax;
}

double EntityParameters::lossToleranceFactor(std::string parameter) {
    if (parameter=="min")
        return lossToleranceFactorMin;
    else
        return lossToleranceFactorMax;
}

//----------------------------------------------------------------------------------

MiningParameters::MiningParameters() {
    creationTime = 0;   // 1473628916
    miningTimeMean = 600; // in Seconds
    miningTimeStd = 300;
    minerMachinePricePerTh = 35;
}

double MiningParameters::getMiningTime(std::string parameter) {
    if (parameter=="mean")
        return miningTimeMean;
    else
        return miningTimeStd;
}

unsigned long MiningParameters::getCreationTime() {
    return creationTime;
}

int MiningParameters::getAverageMiningTime() {
    return miningTimeMean;
}

double MiningParameters::getMinerMachinePricePerTh() {
    return minerMachinePricePerTh;
}

//----------------------------------------------------------------------------------

PopulationParameters::PopulationParameters() {
    minersCarryingCapacity = 67256;
    zeroRevenuePopulation = double(minersCarryingCapacity)*0.75;
    defaultNumberOfPool = 32;
    minersPopulationGrowthRate = 0.11;
}

//----------------------------------------------------------------------------------

long VariableParameters::getCurrentTotalHashPower() {
    return currentTotalHashPower;
}

long VariableParameters::getCurentMinersPopulation() {
    return currentMinersPopulation;
}

long VariableParameters::getCurrentPoolsPopulation() {
    return currentPoolsPopulation;
}

double VariableParameters::getUnitPrice() {
    return unitPrice;
}

double VariableParameters::getUnitPerNewBlock() {
    return unitPerNewBlock;
}

