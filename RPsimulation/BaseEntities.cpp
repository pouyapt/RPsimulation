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

SineWaveModulator::SineWaveModulator(double range, long minPeriod, long maxPeriod) {
    range_ = range/2;
    if (range<=0)
        exit(1);
    d = 0;
    while (range<1000) {
        range *= 10;
        d++;
    }
    maxR = range;
    minP = 0.5*minPeriod;
    maxP = 0.5*maxPeriod;
    x_e = V->getCurrentTime();
    y_e = 0;
    generateParameters();
}

double SineWaveModulator::mainFunction(long t) {
    return r * sin(l*0.5*M_PI*(t-a)) - b;
}

void SineWaveModulator::generateParameters() {
    double l_ = (gen->random_uniform_long(minP, maxP));
    long r_ = gen->random_uniform_long(-maxR/8, maxR/8);
    l = 1/l_;
    r = r_ * l_/maxP * pow(10, -d);
    a = x_e - (1/l);
    b = r - y_e;
    x_e += (2/l);
    double y_e_new = y_e - (2*r);
    if (abs(y_e_new) >= range_) {
        r = -r;
        b = r - y_e;
        y_e_new = y_e - (2*r);
    }
    y_e = y_e_new;
}

double SineWaveModulator::getValue() {
    if (V->getCurrentTime() > x_e) {
        do
            generateParameters();
        while (V->getCurrentTime() > x_e);
    }
    return mainFunction(V->getCurrentTime());
}

//----------------------------------------------------------------------------------

MasterTime::MasterTime() {
    readFile();
}

MasterTime::~MasterTime() {
    writeFile();
    for (auto &ell: M) {
        delete ell.second;
    }
}

void MasterTime::writeFile() {
    std::ofstream out;
    out.open(file);
    out << M.size() << std::endl;
    for (auto &ell: M) {
        out << ell.first << std::endl;
        out << ell.second->maxP << std::endl;
        out << ell.second->minP << std::endl;
        out << ell.second->maxR << std::endl;
        out << ell.second->l << std::endl;
        out << ell.second->r << std::endl;
        out << ell.second->a << std::endl;
        out << ell.second->b << std::endl;
        out << ell.second->x_e << std::endl;
        out << ell.second->y_e << std::endl;
        out << ell.second->range_ << std::endl;
        out << ell.second->d << std::endl;
    }
    out << ModulatorNames.size() << std::endl;
    for (auto i : ModulatorNames)
        out << i << std::endl;
    out.close();
    std::cout << "Master Time file has been saved.\n";
}

bool MasterTime::readFile() {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "Master Time file did not open.\n";
        return false;
    }
    int size = 0;
    in >> size;
    while (size) {
        SineWaveModulator* newM = new SineWaveModulator;
        std::string k;
        in >> k;
        in >> newM->maxP;
        in >> newM->minP;
        in >> newM->maxR;
        in >> newM->l;
        in >> newM->r;
        in >> newM->a;
        in >> newM->b;
        in >> newM->x_e;
        in >> newM->y_e;
        in >> newM->range_;
        in >> newM->d;
        M[k] = newM;
        size--;
    }
    in >> size;
    while (size) {
        std::string temp;
        in >> temp;
        ModulatorNames.push_back(temp);
        size--;
    }
    in.close();
    return true;
}

void MasterTime::applyModulators() {
    for (auto &ell: M) {
        ell.second->getValue();
    }
}

long MasterTime::getCurrentTime() {
    applyModulators();
    return VT->getCurrentTime();
}

long MasterTime::addSecondsToCurrentTime(long seconds) {
    VT->addSecondsToCurrentTime(seconds);
    applyModulators();
    addModValuesToCsvFile();
    return VT->getCurrentTime();
}

long MasterTime::addHoursToCurrentTime(double hours) {
    return VT->addHoursToCurrentTime(hours);
}

void MasterTime::createNewModulator(double range, long minPeriod, long maxPeriod, std::string key) {
    SineWaveModulator* m = new SineWaveModulator(range, minPeriod, maxPeriod);
    M[key] = m;
    ModulatorNames.push_back(key);
}

double MasterTime::getModulatorValue(std::string key) {
    return M[key]->getValue();
}

void MasterTime::CsvFileInit() {
    if (!file_exist(CsvFile)) {
        std::ofstream out;
        out.open(CsvFile);
        out << "unix_time" << ",";
        for (auto i=0; i<ModulatorNames.size(); i++) {
            out << ModulatorNames[i];
            if (i!=ModulatorNames.size()-1)
                out << ",";
        }
        out << std::endl;
        out.close();
    }
}

void MasterTime::addModValuesToCsvFile() {
    CsvFileInit();
    std::fstream uidlFile(CsvFile, std::fstream::in | std::fstream::out | std::fstream::app);
     if (uidlFile.is_open()) {
         uidlFile << getCurrentTime() << ",";
         for (auto i=0; i<ModulatorNames.size(); i++) {
             uidlFile << M[ModulatorNames[i]]->getValue();
             if (i!=ModulatorNames.size()-1)
                 uidlFile << ",";
         }
         uidlFile << std::endl;
         uidlFile.close();
     }
     else {
       std::cout << "Cannot save to 'modulatorValues.csv' file" << std::endl;
     }
}

//----------------------------------------------------------------------------------

Miner::Miner(char option) {
    initialize();
    if (option=='b')
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
    return joinTimestamp;
}

int Miner::getHashPower() {
    return hashPower;
}

int Miner::roundPlayed() {
    return lastPlayedRound - joinedRound;
}

double& Miner::getReputation() {
    return r.reputation;
}

bool Miner::isInPool() {
    return inPool;
}

int Miner::getMined() {
    return powCount;
}
void Miner::updateRoundsPlayed() {
    lastPlayedRound++;
    currentPoolCycle.rounds++;
}

Money Miner::getPowerCostPerHour() {
    return powerCostPerHour;
}

long Miner::getIndex() {
    return index;
}

miningCycle& Miner::getMiningCycle() {
    return currentPoolCycle;
}

double Miner::getExpectedMinedBlocks() {
    return expectedPowCount;
}

void Miner::savePoolManager(PoolManager* poolManager) {
    if (poolJoined==-1)
        poolJoined = T->getCurrentTime();
    pool = poolManager;
    inPool = true;
}

void Miner::removePoolManager(PoolManager* poolManager) {
    if (poolManager == pool) {
        pool = NULL;
        inPool = false;
        poolJoined = -1;
        return;
    }
    std::cout << "Miner::removePoolManager: Pool Manger was not found" << std::endl;
}

void Miner::initialize() {
    idValue = 0;
    joinTimestamp = 0;
    joinedRound = 0;
    lastPlayedRound = 0;
    BWAttackRounds = 0;
    allBWAttacks = 0;
    poolCosts = 0;
    incomePool = 0;
    incomePow = 0;
    incomeDishonesty = 0;
    hashPower = 0;
    dishonestyFactor = 0;
    expectedPowCount = 0;
    powCount = 0;
    poolJoined = -1;
    powerConRate = 0;
    receivedInvitationsCount = 0;
    poolRemoved = 0;
    BW_assigned = false;
    newMiner = true;
    inPool = false;
    pool = NULL;
}

void Miner::resetGlobalPars() {
    T = &VirtualTime::instance();
    gen = &core::Random::instance();
    miningP = &MiningParameters::instance();
    variableP = &Stats::instance();
    pool = NULL;
    inPool = false;
    invitations.clear();
}

void Miner::generateInitialValues() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1000000000000000, 2000000000000000);
    joinTimestamp = T->getCurrentTime();
    poolFeeSensetive = gen->select_random_index(0, 3)==1 ? false : true;
    leaveIfReachProfitTh = gen->select_random_index(0, 2)==1 ? false : true;
    neverJoinPool = gen->select_random_index(0, 14)!=1 ? false : true;
    hashPower = gen->minig_power();
    dishonestyFactor = gen->dishonestyFactor();
    powerConRate = gen->miningPowerConsumption();
    m = machines.randomPick();
    probabilityConfidence = gen->probabilityConfidence();
    investment = hashPower * miningP->getMinerMachinePricePerTh();
    lossTolerance = gen->lossTolerance(investment);
    targetProfit = investment + gen->targetProfit(investment);
    powerCostCalculator();
}

void Miner::resetRoundCycle() {
    currentPoolCycle.BWAttacks = false;
    currentPoolCycle.expectedPowCount = 0;
    currentPoolCycle.powCount = 0;
    currentPoolCycle.rounds = 0;
}

void Miner::powerCostCalculator() {
    double x = (double(hashPower)*double(m.wattage)/double(m.hashRate)) / 1000;
    powerCostPerHour = powerConRate * x;
}

void Miner::receivePoolRewards(double amount) {
    incomePool += amount;
}

void Miner::receivePowRewards(double amount) {
    incomePow += amount;
    powCount++;
    currentPoolCycle.powCount++;
}

void Miner::receiveBribe(double amount) {
    incomeDishonesty += amount;
}

void Miner::addCost(Money amount) {
    if (isInPool())
        poolCosts -= amount;
    else
        soloCosts -= amount;
}

void Miner::print() {
    std::cout << "------------------------------------------\n";
    std::cout << "Name:             " << firstName << " " << lastName << std::endl;
    std::cout << "ID:               " << idValue << std::endl;
    std::cout << "Join Time:        " << convertToDate_Time(joinTimestamp);
    std::cout << "Hash Rate:        " << hashPower << " TH/S" << std::endl;
    std::cout << "Reputation:       " << r.reputation << std::endl;
    std::cout << "DH Factor:        " << dishonestyFactor << std::endl;
    std::cout << "Power Rate:       " << powerConRate*-1 << " per KW" << std::endl;
    std::cout << "Power Cost:       " << powerCostPerHour*-1 << " per Hour" << std::endl;
    std::cout << "Es. Daily Profit: " << estimateDailySoloRevenue(variableP->getUnitPerNewBlock()) << std::endl;
    if (pool!=NULL)
        std::cout << "Member of Pool:   " << pool->poolName() << std::endl;
    else
        std::cout << "Member of Pool:   " << "--\n";
    if (poolJoined!=-1)
        std::cout << "Pool Join Time:   " << convertToDate_Time(poolJoined);
    else
        std::cout << "Pool Join Time:   " << "--\n" ;
    std::cout << "Income - Pool:    " << variableP->getUnitPrice()*incomePool << std::endl;
    std::cout << "Income - Solo:    " << variableP->getUnitPrice()*incomePow << std::endl;
    std::cout << "Income - Cheat:   " << variableP->getUnitPrice()*incomeDishonesty << std::endl;
    std::cout << "Solo Costs:       " << soloCosts << std::endl;
    std::cout << "Pool Costs:       " << poolCosts << std::endl;
    std::cout << "Net Profit:       " << poolCosts + (variableP->getUnitPrice()*(incomePool + incomePow + incomeDishonesty)) << std::endl;
    std::cout << "Mining Time:      " << minedTime << std::endl;
    std::cout << "Blocks Mined:     " << powCount << std::endl;
    std::cout << "Loss Tolerance:   " << lossTolerance << std::endl;
    std::cout << "Target Profit:    " << targetProfit << std::endl;
    std::cout << "Invited to Pools: " << receivedInvitationsCount << std::endl;
    //std::cout << "D Violation       " << detectedViolations << std::endl;
    std::cout << "BW Attacks:       " << BWAttackRounds << std::endl;
    //std::cout << "Machine Name:     " << m.name << std::endl;
    //std::cout << "Machine Hash:     " << m.hashRate << std::endl;
    //std::cout << "Machine Wattage:  " << m.wattage << std::endl;
    std::cout << "------------------------------------------\n";
}

void Miner::printInvitations() {
    for (auto i=0; i<invitations.size(); i++)
        invitations[i]->print();
}

void Miner::receiveInvitation(PoolManager* p) {
    if (p==pool)
        return;
    for (auto i=0; i<invitations.size(); i++) {
        if (invitations[i]==p)
            return;
    }
    invitations.push_back(p);
    receivedInvitationsCount++;
}

Money Miner::getProfitInDollar() {
    Money allIncome;
    allIncome = variableP->getUnitPrice() * (incomePow + incomePool + incomeDishonesty);
    return allIncome + poolCosts + soloCosts;
}

double Miner::getProfitInCrypto() {
    double allIncome = incomePow + incomePool + incomeDishonesty;
    double costsInCrypto = (poolCosts.convert()+soloCosts.convert()) / variableP->getUnitPrice().convert();
    return allIncome + costsInCrypto;
}

bool Miner::soloMiningIsRisky(double dailyPowPx) {
    if (neverJoinPool)
        return false;
    double avePowDaysCycle = 1/dailyPowPx;
    Money projectedCost;
    projectedCost = powerCostPerHour * (24*avePowDaysCycle*(-1));
    if (projectedCost + soloCosts + incomePow <= lossTolerance*0.3)
        return true;
    return false;
}

double Miner::dailyPowProbability() {
    double px = hashPower / double(variableP->getCurrentTotalHashPower());
    int n = 86400 / miningP->getAverageMiningTime();
    return px*n;
}

double Miner::estimateDailySoloRevenue(double reward) {
    double dailyPowPx = dailyPowProbability();
    double dailyRevenue = reward*dailyPowPx;
    return dailyRevenue;
}

double Miner::getDailyCostInCrypto() {
    return (powerCostPerHour.convert()*(-24)) / double(variableP->getUnitPrice().convert());
}

void Miner::joinNewPool(std::vector<PoolManager*> & PM) {
    if (inPool || neverJoinPool)
        return;
    int index = selectPool(PM);
    if (index==-1)
        return;
    PM[index]->pickMiner(this);
}

void Miner::joinNewPool() {
    if (inPool || invitations.empty() || neverJoinPool)
        return;
    int index = selectPool(invitations);
    if (index==-1)
        return;
    invitations[index]->pickMiner(this);
    invitations.clear();
}

int Miner::selectPool(const std::vector<PoolManager*> & PM) {
    bool noSoloMinig = soloMiningIsRisky(dailyPowProbability());
    if (!noSoloMinig)
        return -1;
    if (!poolFeeSensetive) {
        int r = gen->select_random_index(0, int(PM.size()-1));
        return r;
    }
    int index = -1;
    double minPoolFee = INT_MAX;
    for (auto i=0; i<PM.size(); i++) {
        if (PM[i]->poolFee() < minPoolFee) {
            minPoolFee = PM[i]->poolFee();
            index = i;
        }
    }
    return index;
}

bool Miner::isBellowLossTolerance () {
    return getProfitInDollar() < lossTolerance ? true:false;
}

bool Miner::isReachedTargetProfit() {
    return (getProfitInDollar() >= targetProfit && leaveIfReachProfitTh) ? true:false;
}

bool Miner::BribeIsAccepted(double bribePercentage, int numberOfRounds) {
    if (variableP->inReputationMode()==false)
        return true;
    Money bribe;
    bribe = variableP->getUnitPrice() * (variableP->getUnitPerNewBlock() * bribePercentage * numberOfRounds);
    if (bribe + getProfitInDollar() >= targetProfit && leaveIfReachProfitTh)
        return true;
    double dRatio = 0;
    if (variableP->getDishonestActivityCount() > 0)
        dRatio = variableP->getDetectedDishonestActivityCount() / double(variableP->getDishonestActivityCount());
    int t = gen->select_random_index(0, int(dRatio*100));
    if (t<=0)
        return true;
    return false;
}

//----------------------------------------------------------------------------------

bool compareID(Miner* a, Miner* b) {
    return (a->idValue <= b->idValue ? true : false);
}

bool compareDfact(Miner* a, Miner* b) {
    return (a->dishonestyFactor <= b->dishonestyFactor ? true : false);
}

bool compareMiningPower(Miner* a, Miner* b) {
    return (a->hashPower >= b->getHashPower() ? true : false);
}

bool compareJoinDate(Miner* a, Miner* b) {
    return (a->joinTimestamp <= b->joinTimestamp ? true : false);
}

bool compareMined(Miner* a, Miner* b) {
    return (a->powCount >= b->powCount ? true : false);
}

bool compareShuffleValue(Miner* a, Miner* b) {
    return (a->shuffleValue <= b->shuffleValue ? true : false);
}

bool compareOldIndex(Miner* a, Miner* b) {
    return (a->oldIndex <= b->oldIndex ? true : false);
}

bool compareBWAttack(Miner* a, Miner* b) {
    return (a->BWAttackRounds >= b->BWAttackRounds ? true : false);
}

bool compareProfit(Miner* a, Miner* b) {
    Stats* variableP = &Stats::instance();
    return ((a->poolCosts+a->soloCosts+(variableP->getUnitPrice()*(a->incomePool+a->incomePow+a->incomeDishonesty))) >= (b->poolCosts+(variableP->getUnitPrice()*(b->incomePool+b->incomePow+b->incomeDishonesty))) ? true : false);
}

bool compareRep(Miner* a, Miner* b) {
    return (a->r.reputation >= b->r.reputation ? true : false);
}

bool compareInvitationCount(Miner* a, Miner* b) {
    return (a->receivedInvitationsCount >= b->receivedInvitationsCount ? true : false);
}

bool compareIndex(PoolManager* a, PoolManager* b) {
    return (a->index <= b->index ? true : false);
}

bool compareHash(PoolManager* a, PoolManager* b) {
    return (a->totalHashPower <= b->totalHashPower ? true : false);
}

compareFunc selectCompareFunc(std::string by) {
    if (by=="df")
        return &compareDfact;
    else if (by=="mp")
        return &compareMiningPower;
    else if (by=="mc")
        return &compareMined;
    else if (by=="sh")
        return &compareShuffleValue;
    else if (by=="pf")
        return &compareProfit;
    else if (by=="bw")
        return &compareBWAttack;
    else if (by=="jd")
        return &compareJoinDate;
    else if (by=="undo")
        return &compareOldIndex;
    else if (by=="id")
        return &compareID;
    else if (by=="in")
        return &compareInvitationCount;
    else if (by=="rp")
        return &compareRep;
    else
        return &compareID;
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
    poolFee = gen->poolPowReward();
    powReward = gen->poolPowReward();
    totalHashPower = 0;
}

unsigned MiningPool::size() {
    return unsigned(miners.size());
}

std::string& MiningPool::poolName() {
    return MiningPool::name;
}


void MiningPool::distributeMinersReward(double amount, Miner* miner) {
    double minerReward = amount * powReward;
    amount -= minerReward;
    miner->receivePowRewards(minerReward);
    for (int i=0; i<miners.size(); i++) {
        double minersShare = amount * (double(miners[i]->getHashPower())/double(totalHashPower));
        miners[i]->receivePoolRewards(minersShare);
    }
}

void MiningPool::pushBack(Miner* miner) {
    miners.push_back(miner);
}

void MiningPool::addMiner(Miner* miner) {
    miners.push_back(miner);
    totalHashPower += miner->getHashPower();
}

bool MiningPool::removeMiner(Miner* miner) {
    for (auto i=0; i<miners.size(); i++) {
        if (miner==miners[i]) {
            totalHashPower -= miners[i]->getHashPower();
            miners.pop(i);
            return true;
        }
    }
    std::cout << "MiningPool::removeMiner: Miner was not found" << std::endl;
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
    poolIncome = 0;
    poolDishonestIncome = 0;
    powCount = 0;
    expectedPowCount = 0;
    dishonestPowCount = 0;
    bribePayed = 0;
    resetAttackDetectionCycle();
    invitationProcessCycle = 1;
    openToNewMiner = true;
}

void PoolManager::generate() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1000000, 9999999);
    dishonest = gen->select_random_index(0, 1);
    Money investment;
    investment = entityP->getPoolStartingInvestment();
    lossTolerance = gen->lossTolerance(investment);
    costPerMiner = gen->costForOneMinerPerHour();
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
    return MiningPool::totalHashPower;
}

double PoolManager::poolFee() {
    return MiningPool::poolFee;
}

double PoolManager::poolPowReward() {
    return MiningPool::powReward;
}

int PoolManager::getPowCount() {
    return powCount;
}

bool PoolManager::isDishonest() {
    return dishonest;
}

Money PoolManager::getProfit() {
    return variableP->getUnitPrice()*poolIncome;
}

bool PoolManager::isOpenToNewMiners() {
    return openToNewMiner;
}

long PoolManager::getIndex() {
    return index;
}

bool PoolManager::BwAttackIsDetected() {
    double lowerBound = confidenceIntervalLowerBound(expectedPowCount, detectionCycle, R->attackDetectionConfidence);
    double p = powCount / double(detectionCycle);
    if (p < lowerBound)
        return true;
    return false;
}

void PoolManager::detectBwMiners(core::list<Miner*> & mList) {
    if (variableP->inReputationMode()==false)
        return;
    if (detectionCycleRemaining==0) {
        if (BwAttackIsDetected()) {
            std::cout << "Attack detected: " << name << std::endl;
            for (auto i=0; i<miners.size(); i++) {
                Miner* temp = miners[i];
                double lowerBound = confidenceIntervalLowerBound(temp->getMiningCycle().expectedPowCount, temp->getMiningCycle().rounds, R->attackDetectionConfidence);
                double p = temp->getMiningCycle().powCount / double(temp->getMiningCycle().rounds);
                if (p < lowerBound) {
                    T.defection(temp->r);
                    mList.push_back(temp);
                    temp->poolRemoved++;
                    releaseMiner(temp);
                }
                else
                    T.cooperation(temp->r);
            }
        }
        resetAttackDetectionCycle();
    }
    else
        detectionCycleRemaining--;
}

void PoolManager::resetAllMinersCycle() {
    for (auto i=0; i<miners.size(); i++)
        miners[i]->resetRoundCycle();
}

void PoolManager::resetAttackDetectionCycle() {
    detectionCycle = gen->select_random_index(R->attackDetectionCycleMin, R->attackDetectionCycleMax);
    detectionCycleRemaining = detectionCycle;
}

void PoolManager::sortMiners(std::string by) {
    miners.sort(selectCompareFunc(by));
}

void PoolManager::receiveReward(double amount, Miner* miner) {
    double newIncome = amount*MiningPool::poolFee;
    amount -= newIncome;
    poolIncome += newIncome;
    powCount++;
    MiningPool::distributeMinersReward(amount, miner);
}

void PoolManager::receiveDishonestReward(Miner *miner, double reward) {
    double newIncome = reward*MiningPool::poolFee;
    reward -= newIncome;
    poolDishonestIncome += newIncome;
    dishonestPowCount++;
    MiningPool::distributeMinersReward(reward, miner);
}

void PoolManager::payBribe(Miner* miner, double minerBribe) {
    miner->receiveBribe(minerBribe);
    bribePayed -= minerBribe;
}

bool PoolManager::pickMiner(Miner* miner) {
    if (miner->isInPool()) {
        std::cout << "pickMiner: Miner is already in a pool.\n";
        return false;
    }
    MiningPool::addMiner(miner);
    miner->savePoolManager(this);
    miner->resetRoundCycle();
    variableP->updateNumberOfPoolMiners(1);
    return true;
}

bool PoolManager::releaseMiner(Miner* miner) {
    for (int i=0; i<BW->list.size(); i++) {
        if (BW->list[i].bribedMiner == miner)
            BW->list.pop(i);
    }
    if (removeMiner(miner)) {
        miner->removePoolManager(this);
        variableP->updateNumberOfPoolMiners(-1);
        return true;
    }
    return false;
}

void PoolManager::updateCost(Time miningTime) {
    double newCost = miningTime.convertToNumber()/3600.0 * costPerMiner.convert() * MiningPool::miners.size();
    cost -= newCost;
}

bool PoolManager::isBellowLossTolerance() {
    return cost + (variableP->getUnitPrice()*poolIncome) <= lossTolerance ? true : false;
}

void PoolManager::releaseAllMiners() {
    for (auto i=0; i<MiningPool::miners.size(); i++) {
        releaseMiner(miners[i]);
    }
}

double PoolManager::estimateDailyRevenueForMiners(int hash, bool newPool, bool includingPowReward) {
    double poolPx = 0;
    double minerShare = 0;
    if (newPool)
        poolPx = (totalHashPower+hash) / double(variableP->getCurrentTotalHashPower());
    else
        poolPx = totalHashPower / double(variableP->getCurrentTotalHashPower());
    double minerPx = hash / double(variableP->getCurrentTotalHashPower());
    int n = 86400 / miningP->getAverageMiningTime();
    if (newPool)
        minerShare = hash / double(totalHashPower+hash);
    else
        minerShare = hash / double(totalHashPower);
    double reward = variableP->getUnitPerNewBlock();
    reward -= (reward*poolFee());
    double powRewardShare = reward*powReward;
    reward -= powRewardShare;
    reward *= minerShare;
    if (includingPowReward)
        return (n*poolPx*reward) + (n*minerPx*powRewardShare);
    else
        return (n*poolPx*reward);
}

void PoolManager::print() {
    std::cout << "------------------------------------------\n";
    std::cout << "Pool Name:            " << poolName() << std::endl;
    std::cout << "ID Number:            " << idValue << std::endl;
    std::cout << "Pool Fee:             " << poolFee()*100 << "%\n";
    std::cout << "POW Award:            " << MiningPool::powReward*100 << "%\n";
    std::cout << "Dishonest:            " << dishonest << std::endl;
    std::cout << "Miners Count:         " << size() << std::endl;
    std::cout << "Hash Power:           " << poolHashPower() << std::endl;
    std::cout << "Hash Share:           " << getHashShareInPercentile() << "%\n";
    std::cout << "Income:               " << variableP->getUnitPrice()*poolIncome << std::endl;
    std::cout << "Costs:                " << cost << std::endl;
    std::cout << "Bribe:                " << variableP->getUnitPrice()*bribePayed << std::endl;
    std::cout << "Mined Blocks Count:   " << powCount << std::endl;
    std::cout << "Loss Tolerance:       " << lossTolerance << std::endl;
    std::cout << "------------------------------------------\n";
}

double PoolManager::getHashShareInPercentile() {
    return double(poolHashPower()) / double(variableP->getCurrentTotalHashPower())*100;
}

void PoolManager::printPoolMiners() {
    for (auto i=0; i<MiningPool::miners.size(); i++) {
        MiningPool::miners[i]->print();
    }
}

void PoolManager::printCandidateMiners() {
    for (auto i=0; i<candidateMinersList.size(); i++)
        candidateMinersList[i]->print();
}

int PoolManager::checkInvitationProcessCycle() {
    int temp = --invitationProcessCycle;
    if (temp==0)
        invitationProcessCycle = gen->select_random_index(R->invitationProcessCycleMin, R->invitationProcessCycleMax);
    return temp;
}

void PoolManager::receiveCandidateMiner(Miner* miner) {
    for (auto i=0; i<MiningPool::miners.size(); i++) {
        if (miner==MiningPool::miners[i])
            return;
    }
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (miner==candidateMinersList[i])
            return;
    }
    candidateMinersList.push_back(miner);
}

void PoolManager::processCandidateMiners() {
    if (candidateMinersList.empty())
        return;
    int size = int(candidateMinersList.size())*R->invitationPercentage;
    int gSize = R->goodMinersP*size;
    int nSize = R->newMinersP*size;
    int bSize = R->badMinersP*size;
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (gSize && candidateMinersList[i]->getReputation() > R->goodMinerLowBound) {
            candidateMinersList[i]->receiveInvitation(this);
            gSize--;
        }
    }
    nSize += gSize;
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (nSize && candidateMinersList[i]->getReputation() <= R->goodMinerLowBound && candidateMinersList[i]->getReputation() >= R->badMinerLowBound) {
            candidateMinersList[i]->receiveInvitation(this);
            nSize--;
        }
    }
    bSize += nSize;
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (bSize && candidateMinersList[i]->getReputation() < R->badMinerLowBound) {
            candidateMinersList[i]->receiveInvitation(this);
            bSize--;
        }
    }
    clearMinersCandidateList();
}

void PoolManager::clearMinersCandidateList() {
    candidateMinersList.clear();
}


//----------------------------------------------------------------------------------

