#include "structures.h"


MinerPopulation::MinerPopulation() {
    databaseInit();
    int population0 = 0;
    if (!readPopulationData())
        population0 = calculateInitialPopulation();
    updateVariableParameters();
    for (auto i=0; i<population0; i++)
        addMiner();
}

MinerPopulation::~MinerPopulation() {
    writePopulationData();
    writeRemovedMiners();
    for (auto i=0; i<mainList.size(); i++) {
        delete mainList[i];
    }
    for (auto i=0; i<removedList.size(); i++) {
        delete removedList[i];
    }
}

unsigned int MinerPopulation::size() {
    return unsigned(mainList.size());
}

Miner* MinerPopulation::operator [](unsigned index) {
    return mainList[index];
}

long MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

int MinerPopulation::getAllViolationsCount() {
    unsigned int sum = 0;
    for (auto i=0; i<mainList.size(); i++) {
        sum +=mainList[i]->allViolations;
    }
    return sum;
}

int& MinerPopulation::getDetectedViolationsCount() {
    return detectedViolationsCount;
}

void MinerPopulation::updateVariableParameters() {
    variableP->current.minersPopulation = mainList.size();
    variableP->current.totalHashPower = totalHashPower_;
    variableP->current.inactiveMinersPopulation = removedMinersCount + removedList.size();
}

void MinerPopulation::writeMinersData(std::ofstream &out) {
    for (auto i=0; i<mainList.size(); i++) {
        out << mainList[i]->firstName << std::endl;
        out << mainList[i]->lastName << std::endl;
        out << mainList[i]->idValue << std::endl;
        out << mainList[i]->joinedTimestamp << std::endl;
        out << mainList[i]->hashPower << std::endl;
        out << mainList[i]->m.name << std::endl;
        out << mainList[i]->m.hashRate << std::endl;
        out << mainList[i]->m.wattage << std::endl;
        out << mainList[i]->powerCostPerHour.convert() << std::endl;
        out << mainList[i]->dishonestyFactor << std::endl;
        out << mainList[i]->powerConRate.convert() << std::endl;
        out << mainList[i]->mined << std::endl;
        out << mainList[i]->minedTime.convertToNumber() << std::endl;
        out << mainList[i]->poolIncome.convert() << std::endl;
        out << mainList[i]->powIncome.convert() << std::endl;
        out << mainList[i]->costs.convert() << std::endl;
        out << mainList[i]->reputation<< std::endl;
        out << mainList[i]->roundsPlayed << std::endl;
        out << mainList[i]->detectedViolations << std::endl;
        out << mainList[i]->allViolations << std::endl;
        out << mainList[i]->probabilityConfidence << std::endl;
        out << mainList[i]->investment.convert() << std::endl;
        out << mainList[i]->lossTolerance.convert() << std::endl;
        out << mainList[i]->receivedInvitations << std::endl;
    }
}

void MinerPopulation::readMinersData(std::ifstream &in, int size) {
    for (auto i=0; i<size; i++) {
        Miner* newItem = new Miner("blank");
        in >> newItem->firstName;
        in >> newItem->lastName;
        in >> newItem->idValue;
        in >> newItem->joinedTimestamp;
        in >> newItem->hashPower;
        in >> newItem->m.name;
        in >> newItem->m.hashRate;
        in >> newItem->m.wattage;
        in >> newItem->powerCostPerHour;
        in >> newItem->dishonestyFactor;
        in >> newItem->powerConRate;
        in >> newItem->mined;
        in >> newItem->minedTime;
        in >> newItem->poolIncome;
        in >> newItem->powIncome;
        in >> newItem->costs;
        in >> newItem->reputation;
        in >> newItem->roundsPlayed;
        in >> newItem->detectedViolations;
        in >> newItem->allViolations;
        in >> newItem->probabilityConfidence;
        in >> newItem->investment;
        in >> newItem-> lossTolerance;
        in >> newItem->receivedInvitations;
        mainList.push_back(newItem);
    }
}

void MinerPopulation::writePopulationData () {
    std::ofstream pd;
    pd.open(populationDataFile);
    pd << mainList.size() << std::endl;
    pd << (removedMinersCount + removedList.size()) << std::endl;
    pd << totalHashPower_ << std::endl;
    pd << allViolationsCount << std::endl;
    pd << detectedViolationsCount << std::endl;
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
            uidlFile << removedList[i]->joinedTimestamp << std::endl;
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
            uidlFile << removedList[i]->receivedInvitations << std::endl;
        }
    }
    uidlFile.close();
}

void MinerPopulation::writeMinersInvitations(Pools* P) {
    P->saveIndex();
    std::ofstream out;
    out.open("Data/invitations.db");
    for (auto i=0; i<mainList.size(); i++) {
        out << mainList[i]->invitations.size() << std::endl;
        for (auto j=0; j<mainList[i]->invitations.size(); j++) {
            out << mainList[i]->invitations[j].PM->getIndex();
            out << mainList[i]->invitations[j].EstimatedDailyProfit.convert();
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
    for (auto i=0; i<mainList.size(); i++) {
        long size = 0;
        in >> size;
        while (size) {
            poolEvaluation PE;
            int index;
            in >> index;
            PE.PM = (*P)[index];
            in >> PE.EstimatedDailyProfit;
            mainList[i]->invitations.push_back(PE);
            size--;
        }
    }
    in.close();
}

void MinerPopulation::sort(std::string by) {
    mainList.sort(selectCompareFunc(by));
}

void MinerPopulation::shuffleList() {
    shuffleValueGen();
    mainList.sort(&compareShuffleValue);
}

void MinerPopulation::undoShuffleList() {
    mainList.sort(&compareOldIndex);
}

void MinerPopulation::printActiveMiners() {
    std::cout << "\n============== Active Miners =============\n";
    for (auto i=0; i<mainList.size(); i++)
        mainList[i]->print();
}

void MinerPopulation::printPopulationStat() {
    std::cout << "Total Hash:          \t" << totalHashPower() << std::endl;
    std::cout << "Active Miners:       \t" << mainList.size() << std::endl;
    std::cout << "Inactive Miners:     \t" << removedList.size() << std::endl;
    std::cout << "Total D-Vilolation:  \t" << detectedViolationsCount << std::endl;
    std::cout << "Total A-Vilolation:  \t" << getAllViolationsCount() << std::endl;
}

void MinerPopulation::addMiner(long time) {
    Miner* newMiner = new Miner;
    if (time!=0)
        newMiner->joinedTimestamp = time;
    mainList.push_back(newMiner);
    newMiner->index = unsigned(mainList.size());
    totalHashPower_ += newMiner->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::deleteMiner(unsigned index) {
    if (mainList[index]->isTaken())
        removeMinerFromPool(mainList[index]);
    Miner* temp = mainList.pop(index);
    removedList.push_back(temp);
    totalHashPower_ -= temp->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::shuffleValueGen() {
    long n = pow(mainList.size(), 3);
    for (auto i=0; i<mainList.size(); i++) {
        mainList[i]->shuffleValue = gen->random_uniform_long(0, n);
        mainList[i]->oldIndex = i;
    }
}

void MinerPopulation::saveIndex() {
    for (int i=0; i<mainList.size(); i++) {
        mainList[i]->index = i;
    }
}

void MinerPopulation::saveOldOrder() {
    for (auto i=0; i<mainList.size(); i++) {
        mainList[i]->oldIndex = i;
    }
}

int MinerPopulation::topHashPower() {
    int h=0;
    for (auto i=0; i<mainList.size(); i++)
        mainList[i]->hashPower > h ? h = mainList[i]->hashPower : false;
    return h;
}

int MinerPopulation::MinersWithAtLeastOneBlock() {
    int count=0;
    for (auto i=0; i<mainList.size(); i++)
        mainList[i]->mined >= 1 ? count++ : false;
    return count;
}

int MinerPopulation::MinersWithProfit() {
    int count=0;
    for (auto i=0; i<mainList.size(); i++)
        (mainList[i]->costs+mainList[i]->powIncome+mainList[i]->poolIncome) > 0 ? count++ : false;
    return count;
}

void MinerPopulation::processPopulationChange(long time, const Game& game) {
    calculateNumberOfNewMiners(time);
    processMinersAddition(time);
    processMinersRemoval();
}

void MinerPopulation::calculateNumberOfNewMiners(long time) {
    if (populationStage==false) {
        double growth = 0;
        for (auto i=1; i<time; i++) {
            growth = populationGrowthPhase1(T->getCurrentTime()+i);
            population += growth;
        }
        if (T->getCurrentTime() > populationP->halfMaximumMinersTime && growth < populationP->maxPopulationGrowth/2)
            populationStage = true;
    }
    else {
        for (auto i=1; i<time; i++) {
            population += populationGrowthPhase2(mainList.size());
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
    return int(expectedP - mainList.size());
}

void MinerPopulation::processMinersRemoval() {
    if (removeLostMinersCountdown==0) {
        removeLostMiners();
        removeLostMinersCountdown = int(gen->random_uniform_long(80, 300));
    }
    else {
        removeLostMinersCountdown--;
    }
    if (removeLosingMinersCountdown==0) {
        removeLosingMinersFromPools();
        removeLosingMinersCountdown = int(gen->random_uniform_long(80, 300));
    }
    else
        removeLosingMinersCountdown--;
}

void MinerPopulation::removeLostMiners() {
    int i=0;
    while (i < mainList.size()) {
        if (mainList[i]->isBellowLossTolerance())
            deleteMiner(i);
        else
            i++;
    }
}

void MinerPopulation::removeLosingMinersFromPools() {
    for (auto i=0; i<mainList.size(); i++) {
        if (mainList[i]->pool!=NULL && mainList[i]->needsToExitPool())
            removeMinerFromPool(mainList[i]);
    }
}

void MinerPopulation::removeMinerFromPool(Miner* miner) {
        miner->pool->releaseMiner(miner);
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
        out << poolList[i]->profit.convert() << std::endl;
        out << poolList[i]->mined << std::endl;
        out << poolList[i]->MiningPool::poolName() << std::endl;
        out << poolList[i]->MiningPool::poolFee << std::endl;
        out << poolList[i]->totalHashPower << std::endl;
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
        in >> newPM->profit;
        in >> newPM->mined;
        in >> newPM->MiningPool::poolName();
        in >> newPM->MiningPool::poolFee;
        in >> newPM->totalHashPower;
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

void Pools::saveIndex() {
    for (int i=0; i<poolList.size(); i++)
        poolList[i]->index = i;
}
