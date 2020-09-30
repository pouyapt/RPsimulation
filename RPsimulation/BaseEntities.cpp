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

SineWaveModulator::SineWaveModulator(double range, int minPeriod, int maxPeriod) {
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
    double temp = r * sin(l*0.5*M_PI*(t-a)) - b;
    return temp;
}

void SineWaveModulator::generateParameters() {
    double l_ = (gen->random_uniform_long(minP, maxP));
    long r_ = gen->random_uniform_long(-maxR/4, maxR/4);
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
    in.close();
    return true;
}

void MasterTime::applyModulators() {
    for (auto &ell: M)
        ell.second->getValue();
}

long MasterTime::getCurrentTime() {
    applyModulators();
    return VT->getCurrentTime();
}

long MasterTime::addSecondsToCurrentTime(long seconds) {
    VT->addSecondsToCurrentTime(seconds);
    applyModulators();
    return VT->getCurrentTime();
}

long MasterTime::addHoursToCurrentTime(double hours) {
    return VT->addHoursToCurrentTime(hours);
}

void MasterTime::createNewModulator(double range, int minPeriod, int maxPeriod, std::string key) {
    SineWaveModulator* m = new SineWaveModulator(range, minPeriod, maxPeriod);
    M[key] = m;
}

double MasterTime::getModulatorValue(std::string key) {
    return M[key]->getValue();
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

int Miner::getHashPower() {
    return hashPower;
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

bool Miner::isInPool() {
    return taken;
}

int Miner::getMined() {
    return mined;
}

Money Miner::getPowerCostPerHour() {
    return powerCostPerHour;
}

long Miner::getIndex() {
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
    else
        std::cout << "Miner::removePoolManager: Pool Manger was not found" << std::endl;
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
    hashPower = 0;
    dishonestyFactor = 0;
    roundsPlayed = 0;
    mined = 0;
    powerConRate = 0;
    receivedInvitationsCount = 0;
    taken = false;
    pool = NULL;
}

void Miner::generateInitialValues() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1000000000000000, 2000000000000000);
    joinedTimestamp = T->getCurrentTime();
    hashPower = gen->minig_power();
    dishonestyFactor = gen->dishonestyFactor();
    powerConRate = gen->miningPowerConsumption();
    m = machines.randomPick();
    probabilityConfidence = gen->probabilityConfidence();
    investment = hashPower * miningP->getMinerMachinePricePerTh();
    lossTolerance = gen->lossTolerance(investment);
    powerCostCalculator();
}

void Miner::powerCostCalculator() {
    double x = (double(hashPower)*double(m.wattage)/double(m.hashRate)) / 1000;
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
    std::cout << "Name:             " << firstName << " " << lastName << std::endl;
    std::cout << "ID:               " << idValue << std::endl;
    std::cout << "Jonied On:        " << convertToDate_Time(joinedTimestamp);
    std::cout << "Hash Rate:        " << hashPower << " TH/S" << std::endl;
    //std::cout << "DH Factor:      " << dishonestyFactor << std::endl;
    std::cout << "Power Rate:       " << powerConRate << " per KW" << std::endl;
    std::cout << "Power Cost:       " << powerCostPerHour << " per Hour" << std::endl;
    if (pool!=NULL)
        std::cout << "Pool Name:        " << pool->poolName() << std::endl;
    else
        std::cout << "Pool Name:        " << "--" << std::endl;
    std::cout << "Income (Pool):    " << poolIncome << std::endl;
    std::cout << "Income (POW):     " << powIncome << std::endl;
    std::cout << "Costs:            " << costs << std::endl;
    Money profit = poolIncome + powIncome + costs;
    std::cout << "Net Profit:       " << profit << std::endl;
    std::cout << "Reputation:       " << reputation << std::endl;
    std::cout << "Mining Time:      " << minedTime << std::endl;
    std::cout << "Blocks Mined:     " << mined << std::endl;
    std::cout << "Loss Tolerance:   " << lossTolerance << std::endl;
    std::cout << "Invited to Pools: " << receivedInvitationsCount << std::endl;
    //std::cout << "D Violation       " << detectedViolations << std::endl;
    //std::cout << "A Violation:      " << allViolations << std::endl;
    //std::cout << "Machine Name:     " << m.name << std::endl;
    //std::cout << "Machine Hash:     " << m.hashRate << std::endl;
    //std::cout << "Machine Wattage:  " << m.wattage << std::endl;
    std::cout << "------------------------------------------\n";
}

void Miner::printInvitations() {
    for (auto i=0; i<invitations.size(); i++)
        invitations[i].PM->print();
}

void Miner::receiveInvitation(PoolManager* p) {
    poolEvaluation PE;
    PE.PM = p;
    invitations.push_back(PE);
    receivedInvitationsCount++;
}

Money Miner::estimatePoolProfit(PoolManager* PM) {
    Money reward;
    reward = PM->estimateDailyRevenue(hashPower);
    double share = double(hashPower) / (double(PM->poolHashPower())+hashPower);
    Money dailyRewardShare;
    dailyRewardShare = reward * share;
    Money dailyCost;
    dailyCost = powerCostPerHour * 24;
    return dailyRewardShare - dailyCost;
}

void Miner::processInvitation() {
    if (invitations.empty())
        return;
    for (int i=0; i<invitations.size(); i++) {
        invitations[i].EstimatedDailyProfit = estimatePoolProfit(invitations[i].PM);
    }
    Money max;
    max = INT_MIN;
    int index = -1;
    for (int i=0; i<invitations.size(); i++) {
        if (invitations[i].EstimatedDailyProfit > max) {
            max = invitations[i].EstimatedDailyProfit;
            index = i;
        }
    }
    if (index!=-1 && max>0) {
        invitations[index].PM->receiveAcceptedInvitation(this);
        invitations.clear();
    }
}

bool Miner::isBellowLossTolerance () {
    return ( (poolIncome+powIncome+costs) < lossTolerance ? true : false );
}

bool Miner::needsToExitPool() {
    double p = gen->poolExitTolerance();
    return ( (poolIncome+powIncome+costs) < lossTolerance*p ? true : false );
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

bool compareProfit(Miner* a, Miner* b) {
    return ((a->poolIncome + a->powIncome + a->costs) >= (b->poolIncome + b->powIncome+  b->costs) ? true : false);
}

bool compareRep(Miner* a, Miner* b) {
    return (a->reputation >= b->reputation ? true : false);
}

bool compareInvitationCount(Miner* a, Miner* b) {
    return (a->receivedInvitationsCount >= b->receivedInvitationsCount ? true : false);
}

bool compareID_p(providedMiners a, providedMiners b) {
    return compareID(a.miner, b.miner);
}

bool compareDfact_p(providedMiners a, providedMiners b) {
    return compareDfact(a.miner, b.miner);
}


bool compareMiningPower_p(providedMiners a, providedMiners b) {
    return compareMiningPower(a.miner, b.miner);
}

bool compareJoinDate_p(providedMiners a, providedMiners b) {
    return compareJoinDate(a.miner, b.miner);
}

bool compareMined_p(providedMiners a, providedMiners b) {
    return compareMined(a.miner, b.miner);
}

bool compareShuffleValue_p(providedMiners a, providedMiners b) {
    return compareShuffleValue(a.miner, b.miner);
}

bool compareOldIndex_p(providedMiners a, providedMiners b) {
    return compareOldIndex(a.miner, b.miner);
}

bool compareDViolation_p(providedMiners a, providedMiners b) {
    return compareDViolation(a.miner, b.miner);
}

bool compareAViolation_p(providedMiners a, providedMiners b) {
    return compareAViolation(a.miner, b.miner);
}

bool compareProfit_p(providedMiners a, providedMiners b) {
    return compareProfit(a.miner, b.miner);
}

bool compareRep_p(providedMiners a, providedMiners b) {
    return compareRep(a.miner, b.miner);
}

bool compareScore(providedMiners a, providedMiners b) {
    return (a.score >= b.score ? true : false);
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
    else if (by=="dv")
        return &compareDViolation;
    else if (by=="av")
        return &compareAViolation;
    else if (by=="jd")
        return &compareJoinDate;
    else if (by=="undo")
        return &compareOldIndex;
    else if (by=="id")
        return &compareID;
    else
        return &compareInvitationCount;
}

double accessRep(providedMiners a) {
    return a.miner->getReputation();
}

int accessMiningPower(providedMiners a) {
    return a.miner->getHashPower();
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
    totalHashPower = 0;
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
        minersShare = amount * (double(miners[i]->getHashPower())/double(totalHashPower));
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
    profit = 0;
    mined = 0;
    openToNewMiner = true;
}

void PoolManager::generate() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1000000, 9999999);
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

long PoolManager::getIndex() {
    return index;
}

void PoolManager::receiveReward(Money amount, Miner* miner) {
    grossIncome += amount;
    Money newProfit;
    newProfit = amount*MiningPool::poolFee;
    profit += newProfit;
    amount -= newProfit;
    mined++;
    MiningPool::distributeMinersReward(amount, miner);
}

bool PoolManager::pickMiner(Miner* miner) {
    if (miner->isInPool())
        return false;
    MiningPool::addMiner(miner);
    miner->savePoolManager(this);
    variableP->updateNumberOfPoolMiners(1);
    return true;
}

bool PoolManager::releaseMiner(Miner* miner) {
    if (MiningPool::removeMiner(miner)) {
        miner->removePoolManager(this);
        variableP->updateNumberOfPoolMiners(-1);
        return true;
    }
    return false;
}

void PoolManager::receiveCandidateMiner(Miner* miner) {
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (miner==candidateMinersList[i].miner)
            return;
    }
    providedMiners p;
    p.miner = miner;
    candidateMinersList.push_back(p);
}

Money PoolManager::estimateDailyRevenue(int hash) {
    double px = (double(totalHashPower)+hash) / double(variableP->getCurrentTotalHashPower());
    int n = 86400 / miningP->getAverageMiningTime();
    double r = n * px * variableP->getUnitPrice().convert() * variableP->getUnitPerNewBlock();
    Money reward;
    reward = r - (MiningPool::poolFee * r);
    return reward;
}

void PoolManager::receiveAcceptedInvitation(Miner* miner) {
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if (miner==candidateMinersList[i].miner) {
            pickMiner(miner);
        }
    }
}

void PoolManager::print() {
    std::cout << "------------------------------------------\n";
    std::cout << "Pool Name:        " << poolName() << std::endl;
    std::cout << "ID Number:        " << idValue << std::endl;
    std::cout << "Pool Fee:         " << poolFee()*100 << "%\n";
    std::cout << "POW Award:        " << MiningPool::powReward*100 << "%\n";
    std::cout << "Miners Count:     " << size() << std::endl;
    std::cout << "Hash Power:       " << poolHashPower() << std::endl;
    std::cout << "Hash Share:       " << double(poolHashPower()) / double(variableP->getCurrentTotalHashPower())*100 << "%\n";
    std::cout << "All Profit:       " << profit << std::endl;
    std::cout << "Mined Count:      " << mined << std::endl;
    std::cout << "------------------------------------------\n";
}

void PoolManager::printPoolMiners() {
    
}

void PoolManager::printCandidateMiners() {
    for (auto i=0; i<candidateMinersList.size(); i++)
        candidateMinersList[i].miner->print();
}

double PoolManager::aveRep() {
    double ave = 0;
    for (auto i=0; i<candidateMinersList.size(); i++)
        ave += candidateMinersList[i].miner->getReputation();
    return ave/candidateMinersList.size();
}

void PoolManager::calculateCandidatesScore() {
    double ave = aveRep();
    for (auto i=0; i<candidateMinersList.size(); i++) {
        double d = candidateMinersList[i].miner->getReputation() - ave;
        candidateMinersList[i].score = candidateMinersList[i].miner->getHashPower() + d*candidateMinersList[i].miner->getHashPower();
    }
}

void PoolManager::processCandidateMiners() {
    calculateCandidatesScore();
    candidateMinersList.sort(&compareScore);
    int n = gen->errorFactor(0.45, 0.05) * candidateMinersList.size();
    for (auto i=0; i<n; i++) {
        candidateMinersList[i].miner->receiveInvitation(this);
        sendInvitationsCount++;
    }
}

void PoolManager::clearMinersCandidateList() {
    candidateMinersList.clear();
}

//----------------------------------------------------------------------------------

