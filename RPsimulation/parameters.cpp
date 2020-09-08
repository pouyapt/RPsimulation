#include "parameters.h"


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

double MiningParameters::getMiningTime(std::string parameter) {
    if (parameter=="mean")
        return miningTimeMean;
    else
        return miningTimeStd;
}

unsigned long MiningParameters::getZeroTimeOffset() {
    return zeroTimeOffset;
}

int MiningParameters::getAverageMiningTime() {
    return miningTimeMean;
}

double MiningParameters::getMinerMachinePricePerTh() {
    return minerMachinePricePerTh;
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
void VariableParameters::updateNumberOfPoolMiners(int i) {
    numberOfPoolMiners += i;
}

