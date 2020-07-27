#include "structures.h"

bool compareID(Miner* a, Miner* b) {
    return (a->idValue <= b->idValue ? true : false);
}

bool compareDfact(Miner* a, Miner* b) {
    return (a->dishonestyFactor <= b->dishonestyFactor ? true : false);
}

bool compareMiningPower(Miner* a, Miner* b) {
    return (a->miningPower >= b->getMiningPower() ? true : false);
}

bool compareJoinDate(Miner* a, Miner* b) {
    return (a->joinedTimestamp <= b->joinedTimestamp ? true : false);
}

bool compareMined(Miner* a, Miner* b) {
    return (a->mined >= b->mined ? true : false);
}

bool compareShuffleValue(Miner* a, Miner* b) {
    return (a->shuffleValue <= b->shuffleValue ? true : false);
}

bool compareOldIndex(Miner* a, Miner* b) {
    return (a->oldIndex <= b->oldIndex ? true : false);
}

bool compareDViolation(Miner* a, Miner* b) {
    return (a->detectedViolations >= b->detectedViolations ? true : false);
}

bool compareAViolation(Miner* a, Miner* b) {
    return (a->allViolations >= b->allViolations ? true : false);
}

//----------------------------------------------------------------------------------

MinerPopulation::MinerPopulation(int population, std::string file) {
    databaseInit();
    this->file = file;
    readMinerPopulation();
    int x=0;
    if (list.size() > DF)
        return;
    if (population!=DF)
        x = population;
    else
        x = gen.new_population();
    for (int i=0; i<x; i++)
       addMiner();
}

MinerPopulation::~MinerPopulation() {
    writeMinerPopulation();
    for (auto i=0; i<list.size(); i++) {
        delete list[i];
    }
}

unsigned int MinerPopulation::size() {
    return unsigned(list.size());
}

Miner* MinerPopulation::operator [](unsigned index) {
    return list[index];
}

void MinerPopulation::updateList() {
    
}

unsigned MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

int MinerPopulation::getAllViolationsCount() {
    unsigned int sum = 0;
    for (int i=0; i<list.size(); i++) {
        sum +=list[i]->allViolations;
    }
    return sum;
}

int& MinerPopulation::getDetectedViolationsCount() {
    return detectedViolationsCount;
}


void MinerPopulation::writeMinerPopulation () {
    std::ofstream out;
    out.open(file);
    out << list.size() << std::endl;
    out << totalHashPower_ << std::endl;
    out << allViolationsCount << std::endl;
    out << detectedViolationsCount << std::endl;
    for (auto i=0; i<list.size(); i++) {
        out << list[i]->firstName << std::endl;
        out << list[i]->lastName << std::endl;
        out << list[i]->idValue << std::endl;
        out << list[i]->joinedTimestamp << std::endl;
        out << list[i]->miningPower << std::endl;
        out << list[i]->dishonestyFactor << std::endl;
        out << list[i]->powerConRate << std::endl;
        out << list[i]->mined << std::endl;
        out << list[i]->profit << std::endl;
        out << list[i]->reputation<< std::endl;
        out << list[i]->roundsPlayed << std::endl;
        out << list[i]->detectedViolations << std::endl;
        out << list[i]->allViolations << std::endl;
    }
    out.close();
    std::cout << "The miner database have been saved." << std::endl << std::endl;
}

bool MinerPopulation::readMinerPopulation () {
    std::ifstream in;
    in.open(file);
    if (in.fail())
    {
        std::cout << "The miner database file did not open." << std::endl;
        return false;
    }
    int size;
    in >> size;
    in >> totalHashPower_;
    in >> allViolationsCount;
    in >> detectedViolationsCount;
    while (size)
    {
        Miner* newItem = new Miner("blank");
        in >> newItem->firstName;
        in >> newItem->lastName;
        in >> newItem->idValue;
        in >> newItem->joinedTimestamp;
        in >> newItem->miningPower;
        in >> newItem->dishonestyFactor;
        in >> newItem->powerConRate;
        in >> newItem->mined;
        in >> newItem->profit;
        in >> newItem->reputation;
        in >> newItem->roundsPlayed;
        in >> newItem->detectedViolations;
        in >> newItem->allViolations;
        list.push_back(newItem);
        size--;
    }
    in.close();
    return true;
}

void MinerPopulation::sort(std::string by) {
    if (by=="undo") {
        list.sort(&compareOldIndex);
        return;
    }
    saveOldOrder();
    if (by=="df")
        list.sort(&compareDfact);
    else if (by=="mp")
        list.sort(&compareMiningPower);
    else if (by=="jd")
        list.sort(&compareJoinDate);
    else if (by=="mc")
        list.sort(&compareMined);
    else if (by=="sh") {
        shuffleValueGen();
        list.sort(&compareShuffleValue);
    }
    else if (by=="dv") {
        list.sort(&compareDViolation);
    }
    else if (by=="av") {
        list.sort(&compareAViolation);
    }
    else
        list.sort(&compareID);
}

void MinerPopulation::print() {
    std::cout << "Hash:\t" << totalHashPower() << std::endl;
    std::cout << "Size:\t" << size() << std::endl;
    std::cout << "dVil:\t" << detectedViolationsCount << std::endl;
    std::cout << "aVil:\t" << getAllViolationsCount() << std::endl;
    for (auto i=0; i<list.size(); i++)
        list[i]->print();
}

void MinerPopulation::addMiner() {
    Miner* newMiner = new Miner;
    list.push_back(newMiner);
    newMiner->index = unsigned(list.size());
    totalHashPower_ += newMiner->getMiningPower();
}

void MinerPopulation::deleteMiner(unsigned index) {
    totalHashPower_ -= list[index]->getMiningPower();
    list.pop(index);
}

void MinerPopulation::shuffleValueGen() {
    for (unsigned i=0; i<list.size(); i++) {
        list[i]->shuffleValue = gen.random_hash(INT_MIN, INT_MAX);
    }
}

void MinerPopulation::saveIndex() {
    for (int i=0; i<list.size(); i++) {
        list[i]->index = i;
    }
}

void MinerPopulation::saveOldOrder() {
    for (int i=0; i<list.size(); i++) {
        list[i]->oldIndex = i;
    }
}

//----------------------------------------------------------------------------------

Pools::Pools(MinerPopulation & DB) {
    R = &DB;
    if (!readPools(*R)) {
        makePools(DP);
    }
}

Pools::~Pools() {
    writePools(*R);
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


void Pools::writePools(MinerPopulation & DB) {
    DB.saveIndex();
    std::ofstream out;
    out.open(poolFile);
    out << poolList.size() << std::endl;
    for (auto i=0; i<poolList.size(); i++) {
        out << poolList[i]->firstName << std::endl;
        out << poolList[i]->lastName << std::endl;
        out << poolList[i]->idValue << std::endl;
        out << poolList[i]->profit << std::endl;
        out << poolList[i]->mined << std::endl;
        out << poolList[i]->MiningPool::poolName() << std::endl;
        out << poolList[i]->MiningPool::poolFee << std::endl;
        out << poolList[i]->TotalhashPower << std::endl;
        out << poolList[i]->MiningPool::minedAmount << std::endl;
        out << poolList[i]->size() << std::endl;
        for (auto j=0; j<poolList[i]->size(); j++) {
            out << poolList[i]->getMiner(j)->getIndex() << std::endl;
        }
    }
    out.close();
    std::cout << "The pool database have been saved." << std::endl << std::endl;
}

bool Pools::readPools (MinerPopulation & DB) {
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
        in >> newPM->firstName;
        in >> newPM->lastName;
        in >> newPM->idValue;
        in >> newPM->profit;
        in >> newPM->mined;
        in >> newPM->MiningPool::poolName();
        in >> newPM->MiningPool::poolFee;
        in >> newPM->TotalhashPower;
        in >> newPM->MiningPool::minedAmount;
        in >> sizeMiners;
        while (sizeMiners) {
            in >> index;
            newPM->pickMiner(DB[index]);
            sizeMiners--;
        }
        poolList.push_back(newPM);
        sizeAll--;
    }
    in.close();
    return true;
}
