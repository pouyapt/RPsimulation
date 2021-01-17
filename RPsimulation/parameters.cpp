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
    if (parameter=="mean")
        return poolFeeMean;
    else if (parameter=="std")
        return poolFeeStd;
    else if (parameter=="min")
        return poolFeeMin;
    else
        return poolFeeMax;
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

double EntityParameters::targetProfitFactor(std::string parameter) {
    if (parameter=="min")
        return targetProfitFactorMin;
    else
        return targetProfitFactorMax;
}

double EntityParameters::leavePoolBeforeLooseFactor(std::string parameter) {
    if (parameter=="mean")
        return leavePoolBeforeLooseFactorMean;
    else if (parameter=="std")
        return leavePoolBeforeLooseFactorStd;
    else if (parameter=="min")
        return leavePoolBeforeLooseFactorMin;
    else
        return leavePoolBeforeLooseFactorMax;
}

int EntityParameters::poolMinumumMembershipTime(std::string parameter) {
    if (parameter=="mean")
        return poolMinumumMembershipTimeMean;
    else if (parameter=="std")
        return poolMinumumMembershipTimeStd;
    else if (parameter=="min")
        return poolMinumumMembershipTimeMin;
    else
        return poolMinumumMembershipTimeMax;
}

int EntityParameters::getPoolStartingInvestment() {
    return poolStartingInvestment;
}

double EntityParameters::poolCostForOneMinerPerHour(std::string parameter) {
    if (parameter=="mean")
        return poolCostForOneMinerPerHourMean;
    else if (parameter=="std")
        return poolCostForOneMinerPerHourStd;
    else if (parameter=="min")
        return poolCostForOneMinerPerHourMin;
    else
        return poolCostForOneMinerPerHourMax;
}

//----------------------------------------------------------------------------------

PopulationParameters::PopulationParameters() {
    folder();
    if (!readFiles()) {
        populationFunctionSteepness = calculateSteepness();
        maxPopulationGrowth = sigmoidDeravative(halfMaximumMinersTime, maximumMiners, populationFunctionSteepness, halfMaximumMinersTime);
    }
}

PopulationParameters::~PopulationParameters() {
    writeFiles();
}

double PopulationParameters::calculateSteepness() {
    return log((1/startingPopulationToMaximumRatio)-1) / halfMaximumMinersTime;
}

bool PopulationParameters::readFiles() {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "Population Parameters file did not open." << std::endl;
        return false;
    }
    in >> maximumMiners;
    in >> halfMaximumMinersTime;
    in >> populationFunctionSteepness;
    in >> defaultNumberOfPool;
    in >> maxPopulationGrowth;
    in.close();
    return true;
}

void PopulationParameters::writeFiles() {
    std::ofstream out;
    out.open(file);
    out << maximumMiners << std::endl;
    out << halfMaximumMinersTime << std::endl;;
    out << populationFunctionSteepness << std::endl;;
    out << defaultNumberOfPool << std::endl;;
    out << maxPopulationGrowth << std::endl;;
    out.close();
    std::cout << "The Population Parameters file has been saved." << std::endl;
}

//----------------------------------------------------------------------------------

MiningParameters::MiningParameters() {
    folder();
    if (!readFiles()) {
        revenueFunctionSteepness = calculateSteepness();
    }
}

MiningParameters::~MiningParameters() {
    writeFiles();
}

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
    return minerMachinePricePerHash;
}

double MiningParameters::calculateSteepness() {
    PopulationParameters* populationP = &PopulationParameters::instance();
    return -log(populationP->maximumMiners*revenueFunctionZeroPoint) / populationP->maximumMiners*revenueFunctionZeroPoint;
}

bool MiningParameters::readFiles() {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "Mining Parameters file did not open." << std::endl;
        return false;
    }
    in >> zeroTimeOffset;
    in >> miningTimeMean;
    in >> miningTimeStd;
    in >> minerMachinePricePerHash;
    in >> revenueRangeFactor;
    in >> revenueFunctionSteepness;
    in >> revenueFunctionZeroPoint;
    in >> priceFluctuationRange;
    in.close();
    return true;
}

void MiningParameters::writeFiles() {
    std::ofstream out;
    out.open(file);
    out << zeroTimeOffset << std::endl;
    out << miningTimeMean << std::endl;
    out << miningTimeStd << std::endl;
    out << minerMachinePricePerHash << std::endl;
    out << revenueRangeFactor << std::endl;
    out << revenueFunctionSteepness << std::endl;
    out << revenueFunctionZeroPoint << std::endl;
    out << priceFluctuationRange << std::endl;
    out.close();
    std::cout << "The Mining Parameters file has been saved." << std::endl;
}

//----------------------------------------------------------------------------------

char* convertToDate_Time(long time) {
    MiningParameters* MP = &MiningParameters::instance();
    time += MP->getZeroTimeOffset();
    return asctime_ct(std::localtime(&time));
}
