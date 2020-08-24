#include "BaseEntities.h"

MinerMachines::MinerMachines() {
    machineFile = "Source/minerMachines.db";
    if (!readFile())
        exit(1);
}

bool MinerMachines::readFile() {
    std::ifstream in;
    in.open(machineFile);
    if (in.fail()) {
        std::cout << "The machine database file did not open." << std::endl;
        return false;
    }
    int size = 0;
    in >> size;
    for (int i=0; i<size; i++) {
        struct machine m;
        in >> m.name;
        in >> m.hashRate;
        in >> m.wattage;
        mList.push_back(m);
    }
    in.close();
    return true;
}

struct machine MinerMachines::randomPick() {
    int index = gen->select_random_index(0, int(mList.size()-1));
    return mList[index];
}

//----------------------------------------------------------------------------------

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

long Miner::getID() {
    return idValue;
}

long Miner::getJoinedTimestamp() {
    return joinedTimestamp;
}

int Miner::getMiningPower(std::string mode) {
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

bool Miner::isTaken() {
    return taken;
}

int Miner::getMined() {
    return mined;
}

Money Miner::getPowerCostPerHour() {
    return powerCostPerHour;
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
    joinedTimestamp = 0;
    reputation = 0;
    detectedViolations = 0;
    allViolations = 0;
    costs = 0;
    poolIncome = 0;
    powIncome = 0;
    miningPower = 0;
    dishonestyFactor = 0;
    roundsPlayed = 0;
    mined = 0;
    powerConRate = 0;
    taken = false;
    pool = NULL;
}

void Miner::generateInitialValues() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(100130118000, 399995232979);
    joinedTimestamp = virtualTime->addSecondsToCurrentTime(gen->select_random_index(61, 4130));
    miningPower = gen->minig_power();
    dishonestyFactor = gen->dishonestyFactor();
    powerConRate = gen->miningPowerConsumption();
    m = machines.randomPick();
    probabilityConfidence = gen->probabilityConfidence();
    investment = miningPower * miningP->getMinerMachinePricePerTh();
    lossTolerance = gen->lossTolerance(investment);
    powerCostCalculator();
}

void Miner::powerCostCalculator() {
    double x = (double(miningPower)*double(m.wattage)/double(m.hashRate)) / 1000;
    powerCostPerHour = powerConRate * x;
}

void Miner::receivePoolRewards(Money amount) {
    poolIncome += amount;
}

void Miner::receivePowRewards(Money amount) {
    powIncome += amount;
}

void Miner::addCost(Money amount) {
    costs -= amount;
}

void Miner::print() {
    std::cout << "------------------------------------------\n";
    std::cout << "Name:    \t\t" << firstName << " " << lastName << std::endl;
    std::cout << "ID:      \t\t" << idValue << std::endl;
    std::cout << "Jonied On:\t\t" << asctime_c(std::localtime(&joinedTimestamp));
    std::cout << "Hash Rate:\t\t" << miningPower << " TH/S" << std::endl;
    //std::cout << "DH Factor:\t\t" << dishonestyFactor << std::endl;
    std::cout << "Power Rate:\t\t" << powerConRate << " per KW" << std::endl;
    std::cout << "Power Cost:\t\t" << powerCostPerHour << " per Hour" << std::endl;
    if (pool!=NULL)
        std::cout << "Pool Name:\t\t" << pool->poolName() << std::endl;
    else
        std::cout << "Pool Name:\t" << " " << std::endl;
    std::cout << "Income (Pool):\t" << poolIncome << std::endl;
    std::cout << "Income (POW):\t" << powIncome << std::endl;
    std::cout << "Costs:     \t\t" << costs << std::endl;
    Money profit = poolIncome + powIncome + costs;
    std::cout << "Net Profit:\t\t" << profit << std::endl;
    std::cout <<  "Reputation:\t\t" << reputation << std::endl;
    std::cout << "Mining Time:\t" << minedTime << std::endl;
    std::cout << "Blocks Mined:\t" << mined << std::endl;
    std::cout << "Loss Tolerance: " << lossTolerance << std::endl;
    //std::cout << "D Violation:\t" << detectedViolations << std::endl;
    //std::cout << "A Violation:\t" << allViolations << std::endl;
    //std::cout << "Machine Name:\t" << m.name << std::endl;
    //std::cout << "Machine Hash:\t" << m.hashRate << std::endl;
    //std::cout << "Machine Wattage:\t" << m.wattage << std::endl;
    std::cout << "------------------------------------------\n";
}

void Miner::receiveInvitation(PoolManager* p) {
    poolEvaluation PE;
    PE.PM = p;
    invitations.push_back(PE);
}

void Miner::acceptInvitation(PoolManager* p) {
    p->pickMiner(this);
}

Money Miner::estimatePoolProfit(PoolManager* PM) {
    Money sharePerPow;
    Money estimatePowerCost;
    Money profitFromPool;
    double dailyPowProb = PM->getDailyPowProbability();
    int nDays = noOfRequiredTrials(dailyPowProb, probabilityConfidence);
    estimatePowerCost = powerCostPerHour * (nDays * 24);
    double reward = (miningP->getUnitPerNewBlock()*miningP->getUnitPrice());
    reward = reward - (reward * PM->poolFee());
    sharePerPow = (double(miningPower) / double(PM->poolHashPower())) * reward;
    profitFromPool = sharePerPow - estimatePowerCost;
    return profitFromPool;
}

void Miner::processInvitation() {
    if (invitations.empty())
        return;
    for (int i=0; i<invitations.size(); i++)
        invitations[i].EstimatedDailyProfit = estimatePoolProfit(invitations[i].PM);
    Money max;
    max = INT_MIN;
    int index = -1;
    for (int i=0; i<invitations.size(); i++) {
        if (invitations[i].EstimatedDailyProfit > max) {
            max = invitations[i].EstimatedDailyProfit;
            index = i;
        }
    }
    if (index!=-1) {
        acceptInvitation(invitations[index].PM);
        invitations.clear();
    }
}

//----------------------------------------------------------------------------------

MiningPool::MiningPool() {
    initialize();
}

Miner* MiningPool::returnMiner(unsigned index) {
    return miners[index];
}

void MiningPool::initialize() {
    name = gen->mining_pool_name();
    poolFee = gen->poolFee();
    powReward = gen->powReward();
    hashSizeProportion = gen->poolSize();
    TotalhashPower = 0;
    grossIncome = 0;
}

unsigned MiningPool::size() {
    return unsigned(miners.size());
}

std::string& MiningPool::poolName() {
    return MiningPool::name;
}


void MiningPool::distributeMinersReward(Money amount, Miner* miner) {
    Money minerReward = amount * powReward;
    amount -= minerReward;
    miner->receivePowRewards(minerReward);
    for (int i=0; i<miners.size(); i++) {
        Money minersShare;
        minersShare = amount * (double(miners[i]->getMiningPower())/double(TotalhashPower));
        miners[i]->receivePoolRewards(minersShare);
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
    computeDesiredHash();
}

void PoolManager::initialize() {
    profit = 0;
    mined = 0;
    openToNewMiner = true;
}

void PoolManager::generate() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1001301, 9893045);
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

long PoolManager::getID() {
    return idValue;
}

std::time_t PoolManager::getEstablishedTime() {
    return establishedTime;
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

Money PoolManager::poolRewards() {
    return MiningPool::grossIncome;
}

int PoolManager::getMined() {
    return mined;
}

Money PoolManager::getProfit() {
    return profit;
}

bool PoolManager::isOpenToNewMiners() {
    return openToNewMiner;
}

void PoolManager::receiveReward(Money amount, Miner* miner) {
    grossIncome += amount;
    Money newProfit;
    newProfit = amount*MiningPool::poolFee;
    profit += newProfit;
    amount -= newProfit;
    mined++;
    MiningPool::distributeMinersReward(amount, miner);
    computeDesiredHash();
}

bool PoolManager::pickMiner(Miner* miner) {
    if (miner->isTaken())
        return false;
    MiningPool::addMiner(miner);
    miner->savePoolManager(this);
    computeDesiredHash();
    return true;
}

bool PoolManager::releaseMiner(Miner* miner) {
    if (MiningPool::removeMiner(miner)) {
        miner->removePoolManager(this);
        computeDesiredHash();
        return true;
    }
    return false;
}

void PoolManager::receiveMinersList(core::list<providedMiners>& list) {
    candidateMinersList = list;
}

void PoolManager::computeDesiredHash() {
    desiredHash = MiningPool::hashSizeProportion * variableP->getCurrentTotalHashPower();
    if (desiredHash >= MiningPool::TotalhashPower)
        openToNewMiner = false;
}

double PoolManager::getDailyPowProbability() {
    double px = double(TotalhashPower) / double(variableP->getCurrentTotalHashPower());
    int n = 86400 / miningP->getAverageMiningTime();
    return atLeastOneOccurencePerNTrial(px, n);
}

void PoolManager::print() {
    std::cout << "------------------------------------------\n";
        std::cout << "Pool Name:\t\t" << poolName() << std::endl;
    std::cout << "ID Number:\t\t" << idValue << std::endl;
    std::cout << "Pool Fee:\t\t" << poolFee() << std::endl;
    std::cout << "POW Award:\t\t" << MiningPool::powReward << std::endl;
    std::cout << "Manager:\t\t" << firstName << " " << lastName << std::endl;
    std::cout << "Miners C:\t\t" << size() << std::endl;
    std::cout << "HashPower:\t\t" << poolHashPower() << std::endl;
    std::cout << "All Profit:\t\t" << profit << std::endl;
    std::cout << "Mined C:\t\t" << mined << std::endl;
    std::cout << "------------------------------------------\n";
}

//----------------------------------------------------------------------------------

