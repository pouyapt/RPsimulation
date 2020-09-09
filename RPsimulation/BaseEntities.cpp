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

SineWaveModulator::SineWaveModulator(double maxDistance, int decimals, int precision, int minPeriod, int maxPeriod) {
    d = decimals;
    p = precision;
    minP = minPeriod;
    maxP = maxPeriod;
    maxD = maxDistance * pow(10, d+p);
    generateAttributes(V->getCurrentTime());
}

double SineWaveModulator::apply() {
    if (V->getCurrentTime()>=nextExterma) {
        offset = lastCalculation;
        while (!generateAttributes(nextExterma));
    }
    return mainFunction(V->getCurrentTime(), generatedTime);
}

bool SineWaveModulator::generateAttributes(long time) {
    long rateDen = gen->random_uniform_long(minP, maxP);
    currentRate = 1 / double(rateDen);
    int range = (double(rateDen)/double(maxP)) * maxD;
    currentDistance = gen->random_uniform_long(-range/2, range/2) * pow(10, -(d+p));
    generatedTime = time;
    nextExterma = calculateNextExterma();
    if (nextExterma <=V->getCurrentTime())
        return false;
    return true;
}

double SineWaveModulator::mainFunction(long x, long a) {
    lastCalculation = currentDistance * sin(currentRate*M_PI*(x-a)) + offset;
    return lastCalculation;
}

double SineWaveModulator::calculateNextExterma() {
    return 1 / (2*currentRate) + generatedTime;
}

//----------------------------------------------------------------------------------

MasterTime::~MasterTime() {
    for (auto i=0; i<M.size(); i++) {
        delete M[i];
    }
}

void MasterTime::applyModulators() {
    for (auto i=0; i<M.size(); i++)
        M[i]->apply();
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

unsigned MasterTime::createNewModulator(double maxDistance, int decimals, int precision, int minPeriod, int maxPeriod) {
    SineWaveModulator* m = new SineWaveModulator(maxDistance, decimals, precision, minPeriod, maxPeriod);
    M.push_back(m);
    return unsigned(M.size()-1);
}

double MasterTime::getModulatorValue(int index) {
    return M[index]->apply();
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
    miningPower = 0;
    dishonestyFactor = 0;
    roundsPlayed = 0;
    mined = 0;
    powerConRate = 0;
    receivedInvitations = 0;
    taken = false;
    pool = NULL;
}

void Miner::generateInitialValues() {
    firstName = gen->name();
    lastName = gen->name();
    idValue = gen->id_number(1000000000000000, 2000000000000000);
    joinedTimestamp = T->getCurrentTime();
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
    std::cout << "Jonied On:\t\t" << convertToDate_Time(joinedTimestamp);
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
    receivedInvitations++;
}

Money Miner::estimatePoolProfit(PoolManager* PM) {
    Money sharePerPow;
    Money estimatePowerCost;
    Money profitFromPool;
    double dailyPowProb = PM->getDailyPowProbability();
    int nDays = noOfRequiredTrials(dailyPowProb, probabilityConfidence);
    estimatePowerCost = powerCostPerHour * (nDays * 24);
    double reward = (variableP->getUnitPerNewBlock()*variableP->getUnitPrice());
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
        invitations[index].PM->receiveAcceptedInvitation(this);
        invitations.clear();
    }
}

bool Miner::isBellowLossTolerance () {
    return ( (poolIncome+powIncome+costs) < lossTolerance ? true : false );
}

bool Miner::needsToExitPool() {
    double p = gen->errorFactor(0.6, 0.06);
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

bool compareProfit(Miner* a, Miner* b) {
    return ((a->poolIncome + a->powIncome + a->costs) >= (b->poolIncome + b->powIncome+  b->costs) ? true : false);
}

bool compareRep(Miner* a, Miner* b) {
    return (a->reputation >= b->reputation ? true : false);
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

compareFunc selectCompareFunc(std::string by) {
    if (by=="df")
        return &compareDfact;
    if (by=="mp")
        return &compareMiningPower;
    if (by=="mc")
        return &compareMined;
    if (by=="sh")
        return &compareShuffleValue;
    if (by=="pf")
        return &compareProfit;
    if (by=="dv")
        return &compareDViolation;
    if (by=="av")
        return &compareAViolation;
    if (by=="jd")
        return &compareJoinDate;
    if (by=="undo")
        return &compareOldIndex;
    return &compareID;
}

double accessRep(providedMiners a) {
    return a.miner->getReputation();
}

int accessMiningPower(providedMiners a) {
    return a.miner->getMiningPower();
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
    for (auto i=0; i<miners.size(); i++) {
        if (miner==miners[i]) {
            TotalhashPower -= miner->getMiningPower();
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

int PoolManager::getIndex() {
    return index;
}

double PoolManager::poolHashPercentage() {
    return MiningPool::hashSizeProportion;
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
    if (miner->pool!=NULL)
        return false;
    MiningPool::addMiner(miner);
    miner->savePoolManager(this);
    computeDesiredHash();
    variableP->updateNumberOfPoolMiners(1);
    return true;
}

bool PoolManager::releaseMiner(Miner* miner) {
    if (MiningPool::removeMiner(miner)) {
        miner->removePoolManager(this);
        computeDesiredHash();
        variableP->updateNumberOfPoolMiners(-1);
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

void PoolManager::receiveAcceptedInvitation(Miner* miner) {
    for (int i=0; i<candidateMinersList.size(); i++) {
        if (miner==candidateMinersList[i].miner) {
            pickMiner(miner);
            candidateMinersList[i].accept = true;
        }
    }
}

void PoolManager::print() {
    std::cout << "------------------------------------------\n";
        std::cout << "Pool Name:\t\t" << poolName() << std::endl;
    std::cout << "ID Number:\t\t" << idValue << std::endl;
    std::cout << "Pool Fee:\t\t" << poolFee() << std::endl;
    std::cout << "POW Award:\t\t" << MiningPool::powReward << std::endl;
    std::cout << "Miners C:\t\t" << size() << std::endl;
    std::cout << "Hash:\t\t\t" << poolHashPower() << std::endl;
    //std::cout << "Hash Share:\t\t" << (poolHashPower()/variableP->getCurrentTotalHashPower())*100 << "%" << std::endl;
    std::cout << "Hash Share:\t\t" << MiningPool::hashSizeProportion*100 << "%" << std::endl;
    std::cout << "HashPower:\t\t" << poolHashPower() << std::endl;
    std::cout << "All Profit:\t\t" << profit << std::endl;
    std::cout << "Mined C:\t\t" << mined << std::endl;
    std::cout << "------------------------------------------\n";
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
        candidateMinersList[i].score = candidateMinersList[i].miner->getMiningPower() + d*candidateMinersList[i].miner->getMiningPower();
    }
}

void PoolManager::processCandidateMiners() {
    calculateCandidatesScore();
    candidateMinersList.sort(&compareScore);
    long invitedMinersHash=0;
    for (auto i=0; i<candidateMinersList.size(); i++) {
        if ((invitedMinersHash + MiningPool::TotalhashPower) >= desiredHash)
            break;
        candidateMinersList[i].miner->receiveInvitation(this);
        candidateMinersList[i].invite = true;
        invitedMinersHash += candidateMinersList[i].miner->getMiningPower();
    }
}

//----------------------------------------------------------------------------------

