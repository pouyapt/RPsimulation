#include "structures.h"


MinerPopulation::MinerPopulation() {
    int population0 = 0;
    if (!readPopulationData()) {
        population0 = calculateInitialPopulation();
        T->createNewModulator(populationP->growthRandomnessRange, populationP->growthRandomnessRangeMinPhase, populationP->growthRandomnessRangeMaxPhase, "growthRandomness");
    }
    updateVariableParameters();
    for (auto i=0; i<population0; i++)
        addMiner();
}

MinerPopulation::~MinerPopulation() {
    writePopulationData();
    writeRemovedMiners();
    for (auto i=0; i<allMinersList.size(); i++) {
        delete allMinersList[i];
    }
    for (auto i=0; i<removedList.size(); i++) {
        delete removedList[i];
    }
}

unsigned int MinerPopulation::size() {
    return unsigned(allMinersList.size());
}

Miner* MinerPopulation::operator [](unsigned index) {
    return allMinersList[index];
}

long MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

int MinerPopulation::getAllViolationsCount() {
    unsigned int sum = 0;
    for (auto i=0; i<allMinersList.size(); i++) {
        sum += allMinersList[i]->allViolations;
    }
    return sum;
}

int& MinerPopulation::getDetectedViolationsCount() {
    return detectedViolationsCount;
}

void MinerPopulation::updateVariableParameters() {
    variableP->current.minersPopulation = allMinersList.size();
    variableP->current.totalHashPower = totalHashPower_;
    variableP->current.inactiveMinersPopulation = removedMinersCount + removedList.size();
    variableP->current.highestMinerReputation = highestMinerReutation;
    variableP->current.lowestMinerReputation = lowestMinerReutation;
}

void MinerPopulation::resetGlobalParameters() {
    populationP = &PopulationParameters::instance();
    miningP = &MiningParameters::instance();
    gen = &core::Random::instance();
    variableP = &Stats::instance();
    T = &MasterTime::instance();
}

void MinerPopulation::writeMinersData(std::ofstream &out) {
    for (auto i=0; i<allMinersList.size(); i++) {
        out << allMinersList[i]->firstName << std::endl;
        out << allMinersList[i]->lastName << std::endl;
        out << allMinersList[i]->idValue << std::endl;
        out << allMinersList[i]->joinTimestamp << std::endl;
        out << allMinersList[i]->hashPower << std::endl;
        out << allMinersList[i]->m.name << std::endl;
        out << allMinersList[i]->m.hashRate << std::endl;
        out << allMinersList[i]->m.wattage << std::endl;
        out << allMinersList[i]->powerCostPerHour.convert() << std::endl;
        out << allMinersList[i]->dishonestyFactor << std::endl;
        out << allMinersList[i]->powerConRate.convert() << std::endl;
        out << allMinersList[i]->mined << std::endl;
        out << allMinersList[i]->poolJoined << std::endl;
        out << allMinersList[i]->minedTime.convertToNumber() << std::endl;
        out << allMinersList[i]->poolIncome.convert() << std::endl;
        out << allMinersList[i]->powIncome.convert() << std::endl;
        out << allMinersList[i]->costs.convert() << std::endl;
        out << allMinersList[i]->corrupted << std::endl;
        out << allMinersList[i]->reputation << std::endl;
        out << allMinersList[i]->reputationTimeOffset << std::endl;
        out << allMinersList[i]->violationTimeOffset << std::endl;
        out << allMinersList[i]->roundsPlayed << std::endl;
        out << allMinersList[i]->detectedViolations << std::endl;
        out << allMinersList[i]->allViolations << std::endl;
        out << allMinersList[i]->probabilityConfidence << std::endl;
        out << allMinersList[i]->investment.convert() << std::endl;
        out << allMinersList[i]->lossTolerance.convert() << std::endl;
        out << allMinersList[i]->targetProfit.convert() << std::endl;
        out << allMinersList[i]->receivedInvitationsCount << std::endl;
        out << allMinersList[i]->newMiner << std::endl;
    }
}

void MinerPopulation::readMinersData(std::ifstream &in, int size) {
    for (auto i=0; i<size; i++) {
        Miner* newItem = new Miner('b');
        in >> newItem->firstName;
        in >> newItem->lastName;
        in >> newItem->idValue;
        in >> newItem->joinTimestamp;
        in >> newItem->hashPower;
        in >> newItem->m.name;
        in >> newItem->m.hashRate;
        in >> newItem->m.wattage;
        in >> newItem->powerCostPerHour;
        in >> newItem->dishonestyFactor;
        in >> newItem->powerConRate;
        in >> newItem->mined;
        in >> newItem->poolJoined;
        in >> newItem->minedTime;
        in >> newItem->poolIncome;
        in >> newItem->powIncome;
        in >> newItem->costs;
        in >> newItem->corrupted;
        in >> newItem->reputation;
        in >> newItem->reputationTimeOffset;
        in >> newItem->violationTimeOffset;
        in >> newItem->roundsPlayed;
        in >> newItem->detectedViolations;
        in >> newItem->allViolations;
        in >> newItem->probabilityConfidence;
        in >> newItem->investment;
        in >> newItem->lossTolerance;
        in >> newItem->targetProfit;
        in >> newItem->receivedInvitationsCount;
        in >> newItem->newMiner;
        allMinersList.push_back(newItem);
    }
}

void MinerPopulation::writePopulationData () {
    std::ofstream pd;
    pd.open(populationDataFile);
    pd << allMinersList.size() << std::endl;
    pd << (removedMinersCount + removedList.size()) << std::endl;
    pd << totalHashPower_ << std::endl;
    pd << allViolationsCount << std::endl;
    pd << detectedViolationsCount << std::endl;
    pd << highestMinerReutation << std::endl;
    pd << lowestMinerReutation << std::endl;
    pd << population << std::endl;
    pd << populationStage << std::endl;
    pd << variableP->current.numberOfPoolMiners << std::endl;
    pd.close();
    std::ofstream md;
    md.open(minersDataFile);
    writeMinersData(md);
    md.close();
    std::cout << "The miner database has been saved." << std::endl;
}

bool MinerPopulation::readPopulationData () {
    std::ifstream pd;
    pd.open(populationDataFile);
    if (pd.fail()) {
        std::cout << "The population data file did not open." << std::endl;
        return false;
    }
    int size;
    pd >> size;
    pd >> removedMinersCount;
    pd >> totalHashPower_;
    pd >> allViolationsCount;
    pd >> detectedViolationsCount;
    pd >> highestMinerReutation;
    pd >> lowestMinerReutation;
    pd >> population;
    pd >> populationStage;
    pd >> variableP->current.numberOfPoolMiners;
    pd.close();
    if (size==0)
        return false;
    std::ifstream md;
    md.open(minersDataFile);
    if (md.fail()) {
        std::cout << "The miners data file did not open." << std::endl;
        return false;
    }
    readMinersData(md, size);
    md.close();
    return true;
}


void MinerPopulation::writeRemovedMiners() {
    if (removedList.empty())
        return;
    std::fstream uidlFile(removedMinersDataFile, std::fstream::in | std::fstream::out | std::fstream::app);
    if (uidlFile.is_open()) {
        for (auto i=0; i<removedList.size(); i++) {
            uidlFile << removedList[i]->firstName << std::endl;
            uidlFile << removedList[i]->lastName << std::endl;
            uidlFile << removedList[i]->idValue << std::endl;
            uidlFile << removedList[i]->joinTimestamp << std::endl;
            uidlFile << removedList[i]->quitTimestamp << std::endl;
            uidlFile << removedList[i]->hashPower << std::endl;
            uidlFile << removedList[i]->m.name << std::endl;
            uidlFile << removedList[i]->m.hashRate << std::endl;
            uidlFile << removedList[i]->m.wattage << std::endl;
            uidlFile << removedList[i]->powerCostPerHour.convert() << std::endl;
            uidlFile << removedList[i]->dishonestyFactor << std::endl;
            uidlFile << removedList[i]->powerConRate.convert() << std::endl;
            uidlFile << removedList[i]->mined << std::endl;
            uidlFile << removedList[i]->minedTime.convertToNumber() << std::endl;
            uidlFile << removedList[i]->poolIncome.convert() << std::endl;
            uidlFile << removedList[i]->powIncome.convert() << std::endl;
            uidlFile << removedList[i]->costs.convert() << std::endl;
            uidlFile << removedList[i]->reputation<< std::endl;
            uidlFile << removedList[i]->roundsPlayed << std::endl;
            uidlFile << removedList[i]->detectedViolations << std::endl;
            uidlFile << removedList[i]->allViolations << std::endl;
            uidlFile << removedList[i]->probabilityConfidence << std::endl;
            uidlFile << removedList[i]->investment.convert() << std::endl;
            uidlFile << removedList[i]->lossTolerance.convert() << std::endl;
            uidlFile << removedList[i]->receivedInvitationsCount << std::endl;
        }
    }
    uidlFile.close();
}

void MinerPopulation::writeMinersInvitations(Pools* P) {
    P->saveIndex();
    std::ofstream out;
    out.open("Data/invitations.db");
    for (auto i=0; i<allMinersList.size(); i++) {
        out << allMinersList[i]->invitations.size() << std::endl;
        for (auto j=0; j<allMinersList[i]->invitations.size(); j++) {
            out << allMinersList[i]->invitations[j].PM->getIndex() << std::endl;
            out << allMinersList[i]->invitations[j].EstimatedDailyProfit.convert() << std::endl;
        }
    }
    out.close();
}

void MinerPopulation::readMinersInvitations(Pools* P) {
    std::ifstream in;
    in.open("Data/invitations.db");
    if (in.fail()) {
        std::cout << "The miner invitations database file did not open." << std::endl;
        return;
    }
    for (auto i=0; i<allMinersList.size(); i++) {
        long size = 0;
        in >> size;
        while (size) {
            poolEvaluation PE;
            int index;
            in >> index;
            PE.PM = (*P)[index];
            in >> PE.EstimatedDailyProfit;
            allMinersList[i]->invitations.push_back(PE);
            size--;
        }
    }
    in.close();
}

void MinerPopulation::sort(std::string by) {
    allMinersList.sort(selectCompareFunc(by));
}

void MinerPopulation::shuffleList() {
    shuffleValueGen();
    allMinersList.sort(&compareShuffleValue);
}

void MinerPopulation::undoShuffleList() {
    allMinersList.sort(&compareOldIndex);
}

void MinerPopulation::printActiveMiners() {
    std::cout << "\n============== Active Miners =============\n";
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->print();
}

void MinerPopulation::printPopulationStat() {
    std::cout << "Total Hash:          \t" << totalHashPower() << std::endl;
    std::cout << "Active Miners:       \t" << allMinersList.size() << std::endl;
    std::cout << "Inactive Miners:     \t" << removedList.size() << std::endl;
    std::cout << "Total D-Vilolation:  \t" << detectedViolationsCount << std::endl;
    std::cout << "Total A-Vilolation:  \t" << getAllViolationsCount() << std::endl;
}

void MinerPopulation::addMiner(long time) {
    Miner* newMiner = new Miner;
    if (time!=0)
        newMiner->joinTimestamp = time;
    allMinersList.push_back(newMiner);
    newMiner->index = unsigned(allMinersList.size());
    totalHashPower_ += newMiner->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::deleteMiner(unsigned index) {
    if (allMinersList[index]->isInPool())
        removeMinerFromPool(allMinersList[index]);
    Miner* temp = allMinersList.pop(index);
    temp->quitTimestamp = T->getCurrentTime();
    removedList.push_back(temp);
    totalHashPower_ -= temp->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::shuffleValueGen() {
    long n = pow(allMinersList.size(), 3);
    for (auto i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->shuffleValue = gen->random_uniform_long(0, n);
        allMinersList[i]->oldIndex = i;
    }
}

void MinerPopulation::saveIndex() {
    for (int i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->index = i;
    }
}

void MinerPopulation::saveOldOrder() {
    for (auto i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->oldIndex = i;
    }
}

int MinerPopulation::topHashPower() {
    int h=0;
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->hashPower > h ? h = allMinersList[i]->hashPower : false;
    return h;
}

int MinerPopulation::MinersWithAtLeastOneBlock() {
    int count=0;
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->mined >= 1 ? count++ : false;
    return count;
}

int MinerPopulation::MinersWithProfit() {
    int count=0;
    for (auto i=0; i<allMinersList.size(); i++)
        (allMinersList[i]->costs+allMinersList[i]->powIncome+allMinersList[i]->poolIncome) > 0 ? count++ : false;
    return count;
}

void MinerPopulation::populationUpdate(long time, const Game& game) {
    calculateNumberOfNewMiners(time);
    processMinersAddition(time);
    processMinersRemoval();
    updateAllReputations();
}

void MinerPopulation::calculateNumberOfNewMiners(long time) {
    double growth = 0;
    if (populationStage==false) {
        for (auto i=1; i<time; i++) {
            growth = populationGrowthPhase1(T->getCurrentTime()+i);
            population += growth + growth*T->getModulatorValue("growthRandomness");
        }
        if (T->getCurrentTime() > populationP->halfMaximumMinersTime && growth < populationP->maxPopulationGrowth/2)
            populationStage = true;
    }
    else {
        for (auto i=1; i<time; i++) {
            growth = populationGrowthPhase2(allMinersList.size());
            population += growth + growth*T->getModulatorValue("growthRandomness");
        }
    }
}

void MinerPopulation::processMinersAddition(long time) {
    while (population > 1) {
        long joinedTime = gen->random_uniform_long(T->getCurrentTime(), T->getCurrentTime()+time);
        addMiner(joinedTime);
        population--;
    }
}

int MinerPopulation::calculateInitialPopulation() {
    long expectedP = sigmoid(T->getCurrentTime(), populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime, 0);
    return int(expectedP - allMinersList.size());
}

void MinerPopulation::processMinersRemoval() {
    if (removeLostMinersCountdown==0) {
        removeLostMiners();
        removeLostMinersCountdown = int(gen->random_uniform_long(20, 100));
    }
    else {
        removeLostMinersCountdown--;
    }
}

void MinerPopulation::removeLostMiners() {
    int i=0;
    while (i < allMinersList.size()) {
        if (allMinersList[i]->isBellowLossTolerance() || allMinersList[i]->isReachedTargetProfit())
            deleteMiner(i);
        else
            i++;
    }
}

void MinerPopulation::removeMinerFromPool(Miner* miner) {
        miner->pool->releaseMiner(miner);
}

double MinerPopulation::populationEstimate(long time) {
    return sigmoid(time, populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime, 0);
}

double MinerPopulation::populationGrowthPhase1(long time) {
    return sigmoidDeravative(time, populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime);
}

double MinerPopulation::populationGrowthPhase2(long population) {
    return sigmoid(population, populationP->maxPopulationGrowth, miningP->revenueFunctionSteepness, populationP->maximumMiners*miningP->revenueFunctionZeroPoint, populationP->maxPopulationGrowth/2);
}

void MinerPopulation::updateAllReputations() {
    for (auto i=0; i<allMinersList.size(); i++)
        updateReputation(allMinersList[i]);
}

void MinerPopulation::updateReputation(Miner* miner) {
    double minersMembershipDuration = minerPresenceDurationInYear(miner);
    miner->reputation = sigmoid(minersMembershipDuration, 2, 1, miner->reputationTimeOffset, 1);
    updateHighestLowestReputation(miner);
}

double MinerPopulation::minerPresenceDurationInYear(Miner* miner) {
    return (T->getCurrentTime() - miner->joinTimestamp) / 31536000.0;
}

void MinerPopulation::applyNegativeReputation(Miner* miner) {
    double minersMembershipDuration = minerPresenceDurationInYear(miner);
    miner->reputationTimeOffset += minersMembershipDuration - log10(minersMembershipDuration);
    updateReputation(miner);
}

void MinerPopulation::updateHighestLowestReputation(Miner* miner) {
    if (miner->reputation > highestMinerReutation) {
        highestMinerReutation = miner->reputation;
        variableP->current.highestMinerReputation = highestMinerReutation;
        return;
    }
    if (miner->reputation < lowestMinerReutation) {
        lowestMinerReutation = miner->reputation;
        variableP->current.lowestMinerReputation = lowestMinerReutation;
        return;
    }
}

//----------------------------------------------------------------------------------

Pools::Pools(int population) {
    if (!readPools()) {
        if (population==INT_MIN)
            makePools(populationP->defaultNumberOfPool);
        else
            makePools(population);
    }
    else
        MP->readMinersInvitations(this);
    updateVariableP();
}

Pools::~Pools() {
    MP->saveIndex();
    MP->writeMinersInvitations(this);
    writePools();
    for (int i=0; i<poolList.size(); i++)
        delete poolList[i];
}

PoolManager* Pools::operator [] (unsigned index) {
    return poolList[index];
}

void Pools::makePools(int number) {
    for (int i=0; i<number; i++) {
        PoolManager* newPM = new PoolManager;
        poolList.push_back(newPM);
    }
}

PoolManager* Pools::getPool (unsigned index) {
    return poolList[index];
}

unsigned int Pools::size() {
    return unsigned(poolList.size());
}

void Pools::print() {
    for (int i=0; i<poolList.size(); i++)
        poolList[i]->print();
}

void Pools::updateVariableP() {
    variableP->current.poolsPopulation = poolList.size();
}

void Pools::writePools() {
    std::ofstream out;
    out.open(poolFile);
    out << poolList.size() << std::endl;
    for (auto i=0; i<poolList.size(); i++) {
        out << poolList[i]->index << std::endl;
        out << poolList[i]->firstName << std::endl;
        out << poolList[i]->lastName << std::endl;
        out << poolList[i]->idValue << std::endl;
        out << poolList[i]->minimumMembershipTime << std::endl;
        out << poolList[i]->income.convert() << std::endl;
        out << poolList[i]->cost.convert() << std::endl;
        out << poolList[i]->costPerMiner.convert() << std::endl;
        out << poolList[i]->lossTolerance.convert() << std::endl;
        out << poolList[i]->mined << std::endl;
        out << poolList[i]->MiningPool::poolName() << std::endl;
        out << poolList[i]->MiningPool::poolFee << std::endl;
        out << poolList[i]->MiningPool::powReward << std::endl;
        out << poolList[i]->totalHashPower << std::endl;
        out << poolList[i]->sendInvitationsCount << std::endl;
        out << poolList[i]->MiningPool::grossIncome.convert() << std::endl;
        out << poolList[i]->size() << std::endl;
        for (auto j=0; j<poolList[i]->size(); j++) {
            out << poolList[i]->getMiner(j)->getIndex() << std::endl;
        }
    }
    out.close();
    std::cout << "The pool database have been saved." << std::endl;
}

bool Pools::readPools () {
    std::ifstream in;
    in.open(poolFile);
    if (in.fail())
    {
        std::cout << "The pool database file did not open." << std::endl;
        return false;
    }
    int sizeAll;
    int sizeMiners;
    int index;
    in >> sizeAll;
    while (sizeAll) {
        PoolManager* newPM = new PoolManager("blank");
        in >> newPM->index;
        in >> newPM->firstName;
        in >> newPM->lastName;
        in >> newPM->idValue;
        in >> newPM->minimumMembershipTime;
        in >> newPM->income;
        in >> newPM->cost;
        in >> newPM->costPerMiner;
        in >> newPM->lossTolerance;
        in >> newPM->mined;
        in >> newPM->MiningPool::poolName();
        in >> newPM->MiningPool::poolFee;
        in >> newPM->MiningPool::powReward;
        in >> newPM->totalHashPower;
        in >> newPM->sendInvitationsCount;
        in >> newPM->MiningPool::grossIncome;
        in >> sizeMiners;
        while (sizeMiners) {
            in >> index;
            newPM->pushBack((*MP)[index]);
            (*MP)[index]->savePoolManager(newPM);
            sizeMiners--;
        }
        poolList.push_back(newPM);
        sizeAll--;
    }
    in.close();
    return true;
}

void Pools::shuffle() {
    long n = pow(poolList.size(), 3);
    for (auto i=0; i<poolList.size(); i++)
        poolList[i]->index = gen->random_uniform_long(0, n);
    poolList.sort(&compareIndex);
}

void Pools::saveIndex() {
    for (auto i=0; i<poolList.size(); i++)
        poolList[i]->index = i;
}

void Pools::removeLostPools() {
    for (auto i=0; i<poolList.size(); i++) {
        if (poolList[i]->isBellowLossTolerance()) {
            poolList[i]->releaseAllMiners();
            poolList.pop(i);
            saveIndex();
        }
    }
}
