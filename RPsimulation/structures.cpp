#include "structures.h"


MinerPopulation::MinerPopulation(int population) {
    databaseInit();
    file = "Data/miners.db";
    file_inactive = "Data/miners_inactive.db";
    if (readMinerPopulation())
        population = 0;
    readMinerPopulation_inactive();
    updateVariableParameters();
    if (population==INT_MIN)
        population = populationP->defaultMinersPopulation;
    for (auto i=0; i<population; i++)
        addMiner();
}

MinerPopulation::~MinerPopulation() {
    writeMinerPopulation();
    writeMinerPopulation_inactive();
    for (auto i=0; i<list.size(); i++) {
        delete list[i];
    }
    for (auto i=0; i<list_inactive.size(); i++) {
        delete list_inactive[i];
    }
}

unsigned int MinerPopulation::size() {
    return unsigned(list.size());
}

Miner* MinerPopulation::operator [](unsigned index) {
    return list[index];
}

void MinerPopulation::updateList() {
    int removedCount = removeLostMiners();
    removeLosingMinersFromPools();
    double n = gen->errorFactor(populationP->minersPopulationGrowth, 0.006)/144;
    int newPopulation = (n * list.size()) + removedCount;
    for (auto i=0; i<newPopulation; i++)
        addMiner();
}

long MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

int MinerPopulation::getAllViolationsCount() {
    unsigned int sum = 0;
    for (auto i=0; i<list.size(); i++) {
        sum +=list[i]->allViolations;
    }
    return sum;
}

int& MinerPopulation::getDetectedViolationsCount() {
    return detectedViolationsCount;
}

void MinerPopulation::updateVariableParameters() {
    variableP->currentMinersPopulation = list.size();
    variableP->currentTotalHashPower = totalHashPower_;
    variableP->currentInactiveMinersPopulation = list_inactive.size();
    variableP->numberOfSoloMiners = soloMiners;
}

void MinerPopulation::writeMinersAttr(core::list<Miner*> &L, std::ofstream &out) {
    out << L.size() << std::endl;
    for (auto i=0; i<L.size(); i++) {
        out << L[i]->firstName << std::endl;
        out << L[i]->lastName << std::endl;
        out << L[i]->idValue << std::endl;
        out << L[i]->joinedTimestamp << std::endl;
        out << L[i]->miningPower << std::endl;
        out << L[i]->m.name << std::endl;
        out << L[i]->m.hashRate << std::endl;
        out << L[i]->m.wattage << std::endl;
        out << L[i]->powerCostPerHour.convert() << std::endl;
        out << L[i]->dishonestyFactor << std::endl;
        out << L[i]->powerConRate.convert() << std::endl;
        out << L[i]->mined << std::endl;
        out << L[i]->minedTime.convertToNumber() << std::endl;
        out << L[i]->poolIncome.convert() << std::endl;
        out << L[i]->powIncome.convert() << std::endl;
        out << L[i]->costs.convert() << std::endl;
        out << L[i]->reputation<< std::endl;
        out << L[i]->roundsPlayed << std::endl;
        out << L[i]->detectedViolations << std::endl;
        out << L[i]->allViolations << std::endl;
        out << L[i]->probabilityConfidence << std::endl;
        out << L[i]->investment.convert() << std::endl;
        out << L[i]->lossTolerance.convert() << std::endl;
        out << L[i]->receivedInvitations << std::endl;
    }
}

void MinerPopulation::readMinersAttr(core::list<Miner*> &L, std::ifstream &in) {
    long size;
    in >> size;
    while (size) {
        Miner* newItem = new Miner("blank");
        in >> newItem->firstName;
        in >> newItem->lastName;
        in >> newItem->idValue;
        in >> newItem->joinedTimestamp;
        in >> newItem->miningPower;
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
        L.push_back(newItem);
        size--;
    }
}

void MinerPopulation::writeMinerPopulation () {
    std::ofstream out;
    out.open(file);
    out << totalHashPower_ << std::endl;
    out << allViolationsCount << std::endl;
    out << detectedViolationsCount << std::endl;
    out << soloMiners << std::endl;
    writeMinersAttr(list, out);
    out.close();
    std::cout << "The miner database have been saved." << std::endl;
}

bool MinerPopulation::readMinerPopulation () {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "The miner database file did not open." << std::endl;
        return false;
    }
    in >> totalHashPower_;
    in >> allViolationsCount;
    in >> detectedViolationsCount;
    in >> soloMiners;
    readMinersAttr(list, in);
    in.close();
    return true;
}

void MinerPopulation::writeMinerPopulation_inactive() {
    std::ofstream out;
    out.open(file_inactive);
    writeMinersAttr(list_inactive, out);
    out.close();
    std::cout << "The inactive miner database have been saved." << std::endl;
}

bool MinerPopulation::readMinerPopulation_inactive() {
    std::ifstream in;
    in.open(file_inactive);
    if (in.fail()) {
        std::cout << "The inactive miner database file did not open." << std::endl;
        return false;
    }
    readMinersAttr(list_inactive, in);
    in.close();
    return true;
}

void MinerPopulation::writeMinersInvitations(Pools* P) {
    P->saveIndex();
    std::ofstream out;
    out.open("Data/invitations.db");
    for (auto i=0; i<list.size(); i++) {
        out << list[i]->invitations.size() << std::endl;
        for (auto j=0; j<list[i]->invitations.size(); j++) {
            out << list[i]->invitations[j].PM->getIndex();
            out << list[i]->invitations[j].EstimatedDailyProfit.convert();
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
    for (auto i=0; i<list.size(); i++) {
        long size = 0;
        in >> size;
        while (size) {
            poolEvaluation PE;
            int index;
            in >> index;
            PE.PM = (*P)[index];
            in >> PE.EstimatedDailyProfit;
            list[i]->invitations.push_back(PE);
            size--;
        }
    }
    in.close();
}

void MinerPopulation::sort(std::string by) {
    list.sort(selectCompareFunc(by));
}

void MinerPopulation::shuffleList() {
    shuffleValueGen();
    list.sort(&compareShuffleValue);
}

void MinerPopulation::undoShuffleList() {
    list.sort(&compareOldIndex);
}

void MinerPopulation::printActiveMiners() {
    std::cout << "\n============== Active Miners =============\n";
    for (auto i=0; i<list.size(); i++)
        list[i]->print();
}

void MinerPopulation::printInactiveMiners() {
    std::cout << "\n============= Inactive Miners ============\n";
    for (auto i=0; i<list_inactive.size(); i++)
    list_inactive[i]->print();
}

void MinerPopulation::printPopulationStat() {
    std::cout << "Total Hash:          \t" << totalHashPower() << std::endl;
    std::cout << "Active Miners:       \t" << list.size() << std::endl;
    std::cout << "Inactive Miners:     \t" << list_inactive.size() << std::endl;
    std::cout << "Total D-Vilolation:  \t" << detectedViolationsCount << std::endl;
    std::cout << "Total A-Vilolation:  \t" << getAllViolationsCount() << std::endl;
}

void MinerPopulation::addMiner() {
    Miner* newMiner = new Miner;
    list.push_back(newMiner);
    newMiner->index = unsigned(list.size());
    totalHashPower_ += newMiner->getMiningPower();
    soloMiners++;
    updateVariableParameters();
}

void MinerPopulation::deleteMiner(unsigned index) {
    totalHashPower_ -= list[index]->getMiningPower();
    if (list[index]->pool!=NULL)
        list[index]->pool->releaseMiner(list[index]);
    else
        soloMiners--;
    list_inactive.push_back(list.pop(index));
    updateVariableParameters();
}

void MinerPopulation::shuffleValueGen() {
    long n = pow(list.size(), 3);
    for (auto i=0; i<list.size(); i++) {
        list[i]->shuffleValue = gen->random_uniform_long(0, n);
        list[i]->oldIndex = i;
    }
}

void MinerPopulation::saveIndex() {
    for (int i=0; i<list.size(); i++) {
        list[i]->index = i;
    }
}

void MinerPopulation::saveOldOrder() {
    for (auto i=0; i<list.size(); i++) {
        list[i]->oldIndex = i;
    }
}

int MinerPopulation::removeLostMiners() {
    int count = 0;
    for (auto i=0; i<list.size(); i++) {
        if (list[i]->isBellowLossTolerance()) {
            deleteMiner(i);
            count++;
        }
    }
    return count;
}

void MinerPopulation::removeLosingMinersFromPools() {
    for (auto i=0; i<list.size(); i++) {
        if (list[i]->pool!=NULL && list[i]->needsToExitPool()) {
            list[i]->pool->releaseMiner(list[i]);
            soloMiners++;
        }
    }
}

int MinerPopulation::topHashPower() {
    int h=0;
    for (auto i=0; i<list.size(); i++)
        list[i]->miningPower > h ? h = list[i]->miningPower : false;
    return h;
}

int MinerPopulation::MinersWithAtLeastOneBlock() {
    int count=0;
    for (auto i=0; i<list.size(); i++)
        list[i]->mined >= 1 ? count++ : false;
    return count;
}

int MinerPopulation::MinersWithProfit() {
    int count=0;
    for (auto i=0; i<list.size(); i++)
        (list[i]->costs+list[i]->powIncome+list[i]->poolIncome) > 0 ? count++ : false;
    return count;
}

//----------------------------------------------------------------------------------

Pools::Pools() {
    if (!readPools()) {
        makePools(populationP->defaultNumberOfPool);
    }
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
    variableP->currentPoolsPopulation = poolList.size();
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
        out << poolList[i]->TotalhashPower << std::endl;
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
        in >> newPM->TotalhashPower;
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
