#include "BaseEntities.h"

Miner::Miner(std::string option) {
    initialize();
    if (option=="blank")
        return;
    generateInitialValues();
}

std::string Miner::getFirstName() {
    return firstName;
}

std::string Miner::getLastName() {
    return lastName;
}

unsigned int Miner::getID() {
    return idValue;
}

long Miner::getJoinedTimestamp() {
    return joinedTimestamp;
}

int Miner::getMiningPower(std::string mode) {
//    if (mode=="mine") {
//        double x = gen.dishonestyThreshold();
//        if (dishonestyFactor > x) {
//            dMiningPower = gen.miningPowerExpander(miningPower);
//            allViolations++;
//            return dMiningPower;
//        }
//    }
    return miningPower;
}

int Miner::getDetectedViolations() {
    return detectedViolations;
}

int Miner::roundPlayed() {
    return roundsPlayed;
}

double Miner::getReputation() {
    return reputation;
}

double Miner::getProfit() {
    return profit;
}

bool Miner::isTaken() {
    return taken;
}

int Miner::getMined() {
    return mined;
}

bool Miner::miningIsEnded() {
    return endMining;
}

int Miner::getIndex() {
    return index;
}

void Miner::savePoolManager(PoolManager* poolManager) {
    pool = poolManager;
    taken = true;
}

void Miner::removePoolManager(PoolManager* poolManager) {
    if (poolManager == pool) {
        pool = NULL;
        taken = false;
    }
}

void Miner::initialize() {
    idValue = 0;
    joinedTimestamp = std::time(nullptr);
    reputation = 0;
    detectedViolations = 0;
    allViolations = 0;
    profit = 0;
    miningPower = 0;
    dishonestyFactor = 0;
    roundsPlayed = 0;
    mined = 0;
    endMining = 0;
    powerConRate = 0;
    taken = false;
    pool = NULL;
}

void Miner::generateInitialValues() {
    firstName = gen.name();
    lastName = gen.name();
    idValue = gen.id_number(1001301180, 3999952329);
    miningPower = gen.minig_power();
    dishonestyFactor = gen.dishonestyFactor();
    powerConRate = gen.miningPowerConsumption();
}

void Miner::receiveRewards(double amount) {
    profit += amount;
}

void Miner::reduceCost(double amount) {
    profit -= amount;
}

void Miner::print() {
    std::cout << "------------------------------------------\n";
    std::cout << "Name:    \t\t" << firstName << " " << lastName << std::endl;
    std::cout << "ID:      \t\t" << idValue << std::endl;
    std::cout << "Jonied:  \t\t" << std::asctime(std::localtime(&joinedTimestamp));
    std::cout << "Hash Rate:\t\t" << miningPower << std::endl;
    std::cout << "DH Factor:\t\t" << dishonestyFactor << std::endl;
    std::cout << "Power Rate:\t\t" << powerConRate << std::endl;
    if (pool!=NULL)
        std::cout << "Pool Name:\t\t" << pool->poolName() << std::endl;
    else
        std::cout << "Pool Name:\t" << " " << std::endl;
    std::cout << "Mined Count:\t" << mined << std::endl;
    std::cout << "Net Profit:\t\t" << profit << std::endl;
    std::cout <<  "Reputation:\t\t" << reputation << std::endl;
    std::cout << "Play Count:\t\t" << roundsPlayed << std::endl;
    std::cout << "D Violation:\t" << detectedViolations << std::endl;
    std::cout << "A Violation:\t" << allViolations << std::endl;
    std::cout << "------------------------------------------\n";
}

//----------------------------------------------------------------------------------

MiningPool::MiningPool() {
    initialize();
}

Miner* MiningPool::returnMiner(unsigned index) {
    return miners[index];
}

void MiningPool::initialize() {
    name = gen.mining_pool_name();
    poolFee = gen.poolFee();
    TotalhashPower = 0;
    grossIncome = 0;
}

unsigned MiningPool::size() {
    return unsigned(miners.size());
}

std::string& MiningPool::poolName() {
    return MiningPool::name;
}


void MiningPool::distributeMinersReward(double amount) {
    for (int i=0; i<miners.size(); i++) {
        double minersShare = (double(miners[i]->getMiningPower())/double(TotalhashPower))*amount;
        miners[i]->receiveRewards(minersShare);
    }
}

void MiningPool::pushBack(Miner* miner) {
    miners.push_back(miner);
}

void MiningPool::addMiner(Miner* miner) {
    miners.push_back(miner);
    TotalhashPower += miner->getMiningPower();
}

bool MiningPool::removeMiner(Miner* miner) {
    for (int i=0; i<miners.size(); i++) {
        if (miner==miners[i]) {
            TotalhashPower -= miner->getMiningPower();
            miners.pop(i);
            return true;
        }
    }
    std::cout << "removeMiner: Miner was not found" << std::endl;
    return false;
}

bool MiningPool::minerIsMember(Miner* miner) {
    for (int i=0; i<miners.size(); i++) {
        if (miners[i]==miner)
            return true;
    }
    return false;
}

//----------------------------------------------------------------------------------

PoolManager::PoolManager(std::string mode) {
    initialize();
    if (mode=="default")
        generate();
}

void PoolManager::initialize() {
    profit = 0;
    mined = 0;
}

void PoolManager::generate() {
    firstName = gen.name();
    lastName = gen.name();
    idValue = gen.id_number(1001301, 9893045);
}

Miner* PoolManager::getMiner(unsigned index) {
    return returnMiner(index);
}

void PoolManager::pushBack(Miner* miner) {
    MiningPool::pushBack(miner);
}

unsigned PoolManager::size() {
    return MiningPool::size();
}

std::string PoolManager::getFirstName() {
    return firstName;
}

std::string PoolManager::getLastName() {
    return lastName;
}

unsigned PoolManager::getID() {
    return idValue;
}

std::string PoolManager::poolName() {
    return MiningPool::poolName();
}

unsigned int PoolManager::poolHashPower() {
    return MiningPool::TotalhashPower;
}

double PoolManager::poolFee() {
    return MiningPool::poolFee;
}

double PoolManager::poolRewards() {
    return MiningPool::grossIncome;
}

int PoolManager::getMined() {
    return mined;
}

double PoolManager::getProfit() {
    return profit;
}

void PoolManager::receiveReward(double amount) {
    grossIncome += amount;
    profit += amount*(MiningPool::poolFee/100);
    amount -= profit;
    mined++;
    MiningPool::distributeMinersReward(amount);
}

bool PoolManager::pickMiner(Miner* miner) {
    if (miner->isTaken())
        return false;
    MiningPool::addMiner(miner);
    miner->savePoolManager(this);
    return true;
}

bool PoolManager::releaseMiner(Miner* miner) {
    if (MiningPool::removeMiner(miner)) {
        miner->removePoolManager(this);
        return true;
    }
    return false;
}

void PoolManager::print() {
    std::cout << "------------------------------------------\n";
        std::cout << "Pool Name:\t\t" << poolName() << std::endl;
    std::cout << "ID Number:\t\t" << idValue << std::endl;
    std::cout << "Pool Fee:\t\t" << poolFee() << std::endl;
    std::cout << "Manager:\t\t" << firstName << " " << lastName << std::endl;
    std::cout << "Miners C:\t\t" << size() << std::endl;
    std::cout << "HashPower:\t\t" << poolHashPower() << std::endl;
    std::cout << "All Profit:\t\t" << profit << std::endl;
    std::cout << "Mined C:\t\t" << mined << std::endl;
    std::cout << "------------------------------------------\n";
}

//----------------------------------------------------------------------------------
