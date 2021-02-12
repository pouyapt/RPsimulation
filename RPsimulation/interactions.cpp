#include "interactions.h"


PoolJoin::PoolJoin() {
    readFile();
}

PoolJoin::~PoolJoin() {
    writeFile();
}

void PoolJoin::run() {
    if (stats->inReputationMode())
        runReputationMode();
    else
        runNormalMode();
}

void PoolJoin::generateCount() {
    poolJoinCount = gen->select_random_index(8000, 10000);
}

void PoolJoin::writeFile() {
    std::ofstream out;
    out.open(file);
    out << poolJoinCount << std::endl;
    out.close();
    std::cout << "The pool interaction data has been saved." << std::endl;
}

bool PoolJoin::readFile() {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "The pool interaction data file did not open." << std::endl;
        return false;
    }
    in >> poolJoinCount;
    in.close();
    return true;
}

void PoolJoin::provideMiners() {
    P->shuffle();
    for (auto i=0; i<P->size(); i++) {
        PoolManager* t = (*P)[i];
        if (t->checkInvitationProcessCycle()==0) {
            int n = int(gen->random_uniform_long(MP->size()/10, MP->size()/5));
            for (auto j=0; j<n; j++) {
                int m = gen->select_random_index(0, MP->size()-1);
                t->receiveCandidateMiner((*MP)[m]);
            }
        }
    }
}

void PoolJoin::sendInvitations() {
    for (auto i=0; i<P->size(); i++)
        (*P)[i]->processCandidateMiners();
}

void PoolJoin::processInvitations() {
    for (auto i=0; i<MP->size(); i++)
        (*MP)[i]->joinNewPool();
}

void PoolJoin::clearCandidateMinersList() {
    for (auto i=0; i<P->size(); i++)
        (*P)[i]->clearMinersCandidateList();
}

void PoolJoin::runNormalMode() {
    int s = MP->size()/100;
    unsigned int n = gen->select_random_index(0, MP->size());
    for (auto i=n; i<n+s; i++) {
        Miner* temp = (*MP)[i];
        if (!temp->isInPool()) {
            std::vector<PoolManager*> pList;
            pList.clear();
            P->shuffle();
            unsigned int r = gen->select_random_index(0, P->size());
            for (auto j=0; j<r; j++)
                pList.push_back(P->getPool(j));
            if (!pList.empty())
                temp->joinNewPool(pList);
        }
    }
}

void PoolJoin::runReputationMode() {
    provideMiners();
    sendInvitations();
    processInvitations();
}

//----------------------------------------------------------------------------------

Game::Game() {
    if (!ReadGameFile()) {
        totalNetworkCosts = 0;
        T->createNewModulator(miningP->priceFluctuationRange, miningP->priceFluctuationMinPhase, miningP->priceFluctuationMaxPhase, "priceModulator");
        T->createNewModulator(miningP->revenueZeroPointRange, miningP->revenueZeroPointMinPhase, miningP->revenueZeroPointMaxPhase, "revenueZeroPoint");
        generateInitialUnitPrice();
        updateModulatedUnitPrice();
    }
    updateVariableParameters();
}

Game::~Game() {
    WriteGameFile();
}

bool Game::ReadGameFile() {
    std::ifstream in;
    in.open(file);
    if (in.fail()) {
        std::cout << "The game data file did not open." << std::endl;
        return false;
    }
    in >> totalNetworkCosts;
    in >> totalMinedBlocks;
    in >> lastRoundDuration;
    in >> lastRoundPowerCost;
    in >> lastGeneratedBlockTimestamp;
    in >> unitPrice;
    in >> modulatedUnitPrice;
    in >> unitsPerBlock;
    in >> priceModulatorIndex;
    in >> dishonestActivityCount;
    in >> detectedDishonestActivityCount;
    in >> falseDetectedDishonestActivityCount;
    in.close();
    return true;
}

void Game::WriteGameFile() {
    std::ofstream out;
    out.open(file);
    out << totalNetworkCosts.convert() << std::endl;
    out << totalMinedBlocks << std::endl;
    out << lastRoundDuration.convertToNumber() << std::endl;
    out << lastRoundPowerCost.convert() << std::endl;
    out << lastGeneratedBlockTimestamp.convertToNumber() << std::endl;
    out << unitPrice.convert() << std::endl;
    out << modulatedUnitPrice.convert() << std::endl;
    out << unitsPerBlock << std::endl;
    out << priceModulatorIndex << std::endl;
    out << dishonestActivityCount << std::endl;
    out << detectedDishonestActivityCount << std::endl;
    out << falseDetectedDishonestActivityCount << std::endl;
    out.close();
    std::cout << "The game data has been saved." << std::endl;
}

void Game::updateVariableParameters(Miner* minedBy) {
    if (minedBy!=nullptr) {
        variableP->current.minedBy = std::to_string(minedBy->idValue);
        if (minedBy->isInPool())
            variableP->current.pool = minedBy->pool->name;
        else
            variableP->current.pool = "-";
    }
    variableP->current.time = T->getCurrentTime();
    variableP->current.totalCost = totalNetworkCosts;
    variableP->current.totalMinedBlocks = totalMinedBlocks;
    variableP->current.unitPrice = modulatedUnitPrice;
    variableP->current.unitPerNewBlock = unitsPerBlock;
    variableP->current.costRewardRatio = currentCostRewardRatio;
    variableP->current.lastGeneratedBlockTime = lastGeneratedBlockTimestamp.convertToNumber();
    variableP->current.dishonestActivityCount = dishonestActivityCount;
    variableP->current.detectedDishonestActivityCount = detectedDishonestActivityCount;
    variableP->current.falseDetectedDishonestActivityCount = falseDetectedDishonestActivityCount;
}

Miner* Game::winerMiner() {
    Miner* winner = NULL;
    long n = gen->random_uniform_long(4, MP->totalHashPower());
    unsigned i = gen->select_random_index(0, MP->size()-1);
    long sum = 0;
    while (1) {
        sum += (*MP)[i]->getHashPower();
        if (sum >= n) {
            winner = (*MP)[i];
            break;
        }
        i++;
    }
    totalMinedBlocks++;
    return winner;
}

void Game::updateMiningEntitiesData() {
    lastRoundPowerCost = 0;
    Time time;
    time = gen->minig_time();
    lastGeneratedBlockTimestamp = time + T->getCurrentTime();
    MP->populationUpdate(time.convertToNumber(), *this);
    //P->removeLostPools();
    lastRoundDuration = time;
    for (auto i=0; i<P->size(); i++) {
        PoolManager* temp = (*P)[i];
        temp->updateCost(time);
        updateExpectedMinedBlocks(temp);
        processBwDetection(temp);
    }
    for (auto i=0; i<MP->size(); i++) {
        Miner* temp = (*MP)[i];
        updateExpectedMinedBlocks(temp);
        Money powerCost;
        Time minedDuration;
        if (temp->joinTimestamp > T->getCurrentTime())
            minedDuration = lastGeneratedBlockTimestamp - temp->joinTimestamp;
        else
            minedDuration = time;
        powerCost = temp->powerCostPerHour * minedDuration.toHour();
        totalNetworkCosts -= powerCost;
        lastRoundPowerCost += powerCost;
        temp->addCost(powerCost);
        temp->updateRoundsPlayed();
        temp->minedTime += minedDuration;
    }
    T->addSecondsToCurrentTime(time.convertToNumber());
}

void Game::mine() {
    double reward = unitsPerBlock;
    updateMiningEntitiesData();
    BW->initializeAttackEntities();
    Miner* winner = winerMiner();
    if (winner->isInPool()==false)
        winner->receivePowRewards(reward);
    else {
        if (!BW->processAttack(winner, reward, dishonestActivityCount))
            winner->pool->receiveReward(reward, winner);
    }
    updateVariableParameters(winner);
    updateUnitPrice();
    updateModulatedUnitPrice();
    variableP->saveSnapShot();
}

void Game::updateUnitPrice() {
    currentCostRewardRatio = costRewardRatio(MP->size());
    Money aveCost;
    aveCost = lastRoundPowerCost.convert() / lastRoundDuration.convertToNumber();
    aveCost = aveCost * miningP->miningTimeMean;
    unitPrice = (aveCost.convert() / unitsPerBlock) * (currentCostRewardRatio + 1);
}

void Game::updateModulatedUnitPrice() {
    modulatedUnitPrice = unitPrice.convert() + (unitPrice.convert()*T->getModulatorValue("priceModulator"));
}

void Game::generateInitialUnitPrice() {
    double p = double(MP->size()) * gen->errorFactor(2, 0.3, true) / unitsPerBlock;
    unitPrice = p;
}

Time Game::lastGeneratedBlockTime() const {
    return lastGeneratedBlockTimestamp;
}

double Game::costRewardRatio(long population) {
    double modulatedRevenueZeroPoint = miningP->revenueFunctionZeroPoint + (miningP->revenueFunctionZeroPoint*T->getModulatorValue("revenueZeroPoint"));
    return sigmoid(population, miningP->revenueRangeFactor, miningP->revenueFunctionSteepness, populationP->maximumMiners*modulatedRevenueZeroPoint, miningP->revenueRangeFactor*0.45);
}

void Game::updateExpectedMinedBlocks(Miner* miner) {
    double px = miner->hashPower/double(variableP->getCurrentTotalHashPower());
    miner->expectedPowCount += px;
    miner->currentPoolCycle.expectedPowCount += px;
}

void Game::updateExpectedMinedBlocks(PoolManager* p) {
    double px = p->totalHashPower/double(variableP->getCurrentTotalHashPower());
    p->expectedPowCount += px;
}

void Game::processBwDetection(PoolManager* P) {
    core::list<Miner*> mList;
    P->detectBwMiners(mList);
    for (auto i=0; i<mList.size(); i++) {
        Miner* temp = mList[i];
        if (temp->getMiningCycle().BWAttacks==true || temp->BWAttackRounds!=0)
            detectedDishonestActivityCount++;
        else
            falseDetectedDishonestActivityCount++;
        temp->resetRoundCycle();
    }
}

//-------------------------------------------------------------------------------------

BW_Attack::BW_Attack() {
    read();
}

BW_Attack::~BW_Attack() {
    write();
}

void BW_Attack::write() {
    std::ofstream out;
    out.open(filename);
    MP->saveIndex();
    P->saveIndex();
    out << BW->list.size() << std::endl;
    for (int i=0; i<BW->list.size(); i++) {
        out << BW->list[i].suspect->getIndex() << std::endl;
        out << BW->list[i].victim->getIndex() << std::endl;
        out << BW->list[i].bribedMiner->getIndex() << std::endl;
        out << BW->list[i].bribePercentage << std::endl;
        out << BW->list[i].rounds << std::endl;
        out << BW->list[i].remainingRounds << std::endl;
    }
    out.close();
    std::cout << "The BW_Attack data has been saved." << std::endl;
}

void BW_Attack::read() {
    std::ifstream in;
    in.open(filename);
    if (in.fail()) {
        std::cout << "The BW_Attack data file did not open." << std::endl;
        return;
    }
    AttackCase temp;
    int i;
    int size;
    in >> size;
    while (size) {
        in >> i;
        temp.suspect = (*P)[i];
        in >> i;
        temp.victim = (*P)[i];
        in >> i;
        temp.bribedMiner = (*MP)[i];
        in >> temp.bribePercentage;
        in >> temp.rounds;
        in >> temp.remainingRounds;
        BW->list.push_back(temp);
        size--;
    }
}

bool BW_Attack::assignVictim() {
    if (P->size() <= 1)
        return false;
    P->shuffle();
    AttackCase temp;
    for (auto i=0; i<P->size(); i++) {
        PoolManager* pm = (*P)[i];
        if (!pm->isDishonest() && pm->getHashShareInPercentile() > BWP->minimumVictimHashShare)
            temp.victim = pm;
    }
    if (temp.victim==nullptr)
        return false;
    for (auto i=0; i<P->size(); i++) {
        PoolManager* pm = (*P)[i];
        if (pm->isDishonest())
            temp.suspect = pm;
    }
    if (temp.suspect==nullptr)
        return false;
    BW->list.push_back(temp);
    return true;
}

bool BW_Attack::selectMinerFromVictimPool() {
    PoolManager* pi = BW->list.end().victim;
    BW->list.end().victim->sortMiners("mp");
    int r = gen->select_random_index(1, BWP->victimPoolAttempts);
    if (r > BW->list.end().victim->size()-1)
        r = BW->list.end().victim->size()-1;
    for (int i=0; i<r; i++) {
        Miner* mi = BW->list.end().victim->getMiner(i);
        double bribePercentage = 0;
        int numberOfRounds = 0;
        if (!mi->BW_assigned && checkMinerHashShare(mi, pi) && minerIsCorrupt(mi, bribePercentage, numberOfRounds)) {
            mi->BW_assigned = true;
            BW->list.end().bribedMiner = mi;
            BW->list.end().rounds = numberOfRounds;
            BW->list.end().remainingRounds = numberOfRounds;
            BW->list.end().bribePercentage = bribePercentage;
            return true;
        }
    }
    BW->list.pop_back();
    return false;
}

bool BW_Attack::minerIsCorrupt(Miner* miner, double & bribePercentage, int & numberOfRounds) {
    double r = gen->select_random_index(0, 99) / 100.0;
    if (r > miner->dishonestyFactor)
        return false;
    bribePercentage = calculateBribePercentage();
    numberOfRounds = gen->select_random_index(1, BWP->maxAttackCounts);
    if (miner->BribeIsAccepted(bribePercentage, numberOfRounds)==false)
        return false;
    return true;
}

bool BW_Attack::checkMinerHashShare(Miner* miner, PoolManager* p) {
    if (miner->hashPower >= p->poolHashPower()*BWP->maxMinerHashShare)
        return false;
    return true;
}

int BW_Attack::getBribedMiner(Miner* miner) {
    for (int i=0; i<BW->list.size(); i++) {
        if (miner==BW->list[i].bribedMiner)
            return i;
    }
    return -1;
}

bool BW_Attack::initializeAttackEntities() {
    if (!assignVictim())
        return false;
    if (!selectMinerFromVictimPool())
        return false;
    return true;
}

double BW_Attack::calculateBribePercentage() {
    return gen->select_random_index(BWP->minerBribeMin, BWP->minerBribeMax)/100.0;
}

bool BW_Attack::processAttack(Miner* & miner, double reward, int & activity) {
    int i = getBribedMiner(miner);
    if (i==-1)
        return false;
    int poolSize = BW->list[i].suspect->size();
    if (!poolSize) {
        BW->list.pop(i);
        return false;
    }
    double bribeAmount = BW->list[i].bribePercentage*reward;
    BW->list[i].suspect->payBribe(miner, bribeAmount);
    if (BW->list[i].remainingRounds == BW->list[i].rounds) {
        miner->BWAttackRounds++;
        miner->getMiningCycle().BWAttacks = true;
        activity++;
    }
    miner->allBWAttacks++;
    BW->list[i].remainingRounds--;
    saveToCsvFile(i, reward, bribeAmount);
    int r = gen->select_random_index(0, poolSize-1);
    Miner* suspect = BW->list[i].suspect->getMiner(r);
    BW->list[i].suspect->receiveDishonestReward(suspect, reward-bribeAmount);
    if (BW->list[i].remainingRounds==0) {
        miner->BW_assigned = false;
        BW->list.pop(i);
    }
    miner = suspect;
    return true;
}

void BW_Attack::print() {
    for (int i=0; i<BW->list.size(); i++) {
        std::cout <<  i << "\n\tXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" << std::endl;
        BW->list[i].bribedMiner->print();
        BW->list[i].victim->print();
        BW->list[i].suspect->print();
    }
}

void BW_Attack::CsvFileInit() {
    if (!file_exist("Output/blockwithholding_event.csv")) {
        std::ofstream out;
        out.open("Output/blockwithholding_event.csv");
        out << "mining_round,"
            << "suspect_pool,"
            << "victim_pool,"
            << "bribed_miner,"
            << "reward_amount,"
            << "bribe_paid\n";
        out.close();
    }
}

void BW_Attack::saveToCsvFile(int i, double reward, double bribe) {
    CsvFileInit();
    std::string filename = "Output/blockwithholding_event.csv";
    std::fstream out(filename, std::fstream::in | std::fstream::out | std::fstream::app);
     if (out.is_open()) {
         out << variableP->current.totalMinedBlocks+1 << ",";
         out << BW->list[i].suspect->poolName() << ",";
         out << BW->list[i].victim->poolName() << ",";
         out << BW->list[i].bribedMiner->idValue << ",";
         out << reward  << ",";
         out << bribe << std::endl;
         out.close();
     }
     else
       std::cout << "Cannot save to 'blockwithholding_event.csv' file" << std::endl;
}

//-------------------------------------------------------------------------------------


void Reputation::cooperation(Miner* miner) {
    
}

void Reputation::defection(Miner* miner) {
    
}

//-------------------------------------------------------------------------------------

void run(int rounds, bool mode) {
    Stats* variableP = &Stats::instance();
    variableP->inReputationMode() = mode;
    Game* G = &Game::instance();
    PoolJoin* PJ = &PoolJoin::instance();
    for (auto i=0; i<rounds; i++) {
        G->mine();
        PJ->run();
    }
    Pools* P = &Pools::instance();
    P->print();
    variableP->printCurrentStats();
}
