#include "interactions.h"


PoolJoin::PoolJoin() {
    if (!readFile())
        generateCount();
}

PoolJoin::~PoolJoin() {
    writeFile();
}

void PoolJoin::generateCount() {
    countUntilProcessInvitations = int(gen->random_uniform_long(5, 15));
}

void PoolJoin::writeFile() {
    std::ofstream out;
    out.open(file);
    out << countUntilProcessInvitations << std::endl;
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
    in >> countUntilProcessInvitations;
    in.close();
    return true;
}

void PoolJoin::provideMiners() {
    P->shuffle();
    for (auto i=0; i<P->size(); i++) {
        int n = int(gen->random_uniform_long(MP->size()/10, MP->size()/5));
        for (auto j=0; j<n; j++) {
            int m = gen->select_random_index(0, MP->size()-1);
            (*P)[i]->receiveCandidateMiner((*MP)[m]);
        }
    }
}

void PoolJoin::sendInvitations() {
    for (auto i=0; i<P->size(); i++)
        (*P)[i]->processCandidateMiners();
}

void PoolJoin::processInvitations() {
    for (auto i=0; i<MP->size(); i++)
        (*MP)[i]->processInvitation();
}

void PoolJoin::clearCandidateMinersList() {
    for (auto i=0; i<P->size(); i++)
        (*P)[i]->clearMinersCandidateList();
}

void PoolJoin::run() {
    provideMiners();
    sendInvitations();
    if (countUntilProcessInvitations)
        countUntilProcessInvitations--;
    else {
        processInvitations();
        clearCandidateMinersList();
        generateCount();
    }
}

//----------------------------------------------------------------------------------

Game::Game() {
    if (!ReadGameFile()) {
        totalNetworkRevenue = 0;
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
    in >> totalNetworkRevenue;
    in >> totalMinedBlocks;
    in >> lastRoundDuration;
    in >> lastRoundPowerCost;
    in >> lastGeneratedBlockTimestamp;
    in >> unitPrice;
    in >> modulatedUnitPrice;
    in >> unitsPerBlock;
    in >> priceModulatorIndex;
    in.close();
    return true;
}

void Game::WriteGameFile() {
    std::ofstream out;
    out.open(file);
    out << totalNetworkCosts.convert() << std::endl;
    out << totalNetworkRevenue.convert() << std::endl;
    out << totalMinedBlocks << std::endl;
    out << lastRoundDuration.convertToNumber() << std::endl;
    out << lastRoundPowerCost.convert() << std::endl;
    out << lastGeneratedBlockTimestamp.convertToNumber() << std::endl;
    out << unitPrice.convert() << std::endl;
    out << modulatedUnitPrice.convert() << std::endl;
    out << unitsPerBlock << std::endl;
    out << priceModulatorIndex << std::endl;
    out.close();
    std::cout << "The game data has been saved." << std::endl;
}

void Game::updateVariableParameters() {
    variableP->current.time = T->getCurrentTime();
    variableP->current.totalRevenue = totalNetworkRevenue;
    variableP->current.totalCost = totalNetworkCosts;
    variableP->current.totalMinedBlocks = totalMinedBlocks;
    variableP->current.unitPrice = modulatedUnitPrice;
    variableP->current.unitPerNewBlock = unitsPerBlock;
    variableP->current.costRewardRatio = currentCostRewardRatio;
    variableP->current.lastGeneratedBlockTime = lastGeneratedBlockTimestamp.convertToNumber();
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

void Game::updateCosts() {
    lastRoundPowerCost = 0;
    Time time;
    time = gen->minig_time();
    lastGeneratedBlockTimestamp = time + T->getCurrentTime();
    MP->populationUpdate(time.convertToNumber(), *this);
    P->removeLostPools();
    lastRoundDuration = time;
    for (auto i=0; i<P->size(); i++) {
        (*P)[i]->updateCost(time);
    }
    for (auto i=0; i<MP->size(); i++) {
        Money powerCost;
        Time minedDuration;
        if ((*MP)[i]->joinTimestamp > T->getCurrentTime())
            minedDuration = lastGeneratedBlockTimestamp - (*MP)[i]->joinTimestamp;
        else
            minedDuration = time;
        powerCost = (*MP)[i]->powerCostPerHour * minedDuration.toHour();
        totalNetworkCosts -= powerCost;
        lastRoundPowerCost += powerCost;
        (*MP)[i]->addCost(powerCost);
        (*MP)[i]->roundsPlayed++;
        (*MP)[i]->minedTime += minedDuration;
    }
    T->addSecondsToCurrentTime(time.convertToNumber());
}

void Game::mine() {
    Money reward;
    reward = modulatedUnitPrice * unitsPerBlock;
    updateCosts();
    Miner* winner = winerMiner();
    if (winner->isInPool()==false)
        winner->receivePowRewards(reward);
    else
        winner->pool->receiveReward(reward, winner);
    winner->mined++;
    totalNetworkRevenue += reward;
    updateVariableParameters();
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

//-------------------------------------------------------------------------------------

bool BW_Attack::assignVictim_Suspect() {
    if (P->size() <= 1)
        return false;
    int r1,r2;
    do {
        r1 = gen->select_random_index(0, P->size()-1);
        r2 = gen->select_random_index(0, P->size()-1);
    }
    while (r1==r2);
    if ((*P)[r1]->size()==0 || (*P)[r2]->size()==0)
        return false;
    if ((*P)[r1]->getHashShare() < 15 && (*P)[r2]->getHashShare() < 15)
        return false;
    PoolManager* v;
    PoolManager* s;
    if ((*P)[r1]->getHashShare() > (*P)[r2]->getHashShare()) {
        v = (*P)[r1];
        s = (*P)[r2];
    }
    else {
        v = (*P)[r2];
        s = (*P)[r1];
    }
    AttackGroup temp;
    temp.suspect = s;
    temp.victim = v;
    list.push_back(temp);
    return true;
}

bool BW_Attack::selectMinerFromVictimPool() {
    list.end().victim->sortMiners("mp");
    for (int i=0; i<3; i++) {
        if (minerIsCorrupt(list.end().victim->getMiner(i))) {
            list.end().bribedMiner = list.end().victim->getMiner(i);
            return true;
        }
    }
    list.pop_back();
    return false;
}

bool BW_Attack::selectMinerFromSuspectPool() {
    list.end().winnerMiner = list.end().suspect->getMiner(0);
    return true;
}

bool BW_Attack::minerIsCorrupt(Miner* miner) {
    double r = gen->select_random_index(0, 99) / 100.0;
    if (r <= miner->dishonestyFactor)
        return true;
    return false;
}

int BW_Attack::getBribedMiner(Miner* miner) {
    for (int i=0; i<list.size(); i++) {
        if (miner==list[i].bribedMiner)
            return i;
    }
    return -1;
}

bool BW_Attack::initializeAttackEntities() {
    if (!assignVictim_Suspect())
        return false;
    if (!selectMinerFromVictimPool())
        return false;
    selectMinerFromSuspectPool();
    return true;
}

Money BW_Attack::calculateBribe(Money reward, Miner* miner) {
    Money poolFee, powReward;
    poolFee = reward * miner->pool->poolFee();
    reward = reward - poolFee;
    powReward = reward * miner->pool->poolPowReward();
    reward = reward - powReward;
    Money minerReward;
    double r = double(miner->hashPower) / double(miner->pool->poolHashPower());
    minerReward = reward * r;
    minerReward += powReward;
    Money bribe;
    bribe = minerReward + (minerReward*0.1);
    return bribe;
}

bool BW_Attack::processAttack(Miner* miner, Money reward) {
    int i = getBribedMiner(miner);
    if (i==-1)
        return false;
    else {
        Money bribe;
        bribe = calculateBribe(reward, miner);
        reward -= bribe;
        list[i].bribedMiner->receivePowRewards(bribe);
        list[i].suspect->receiveReward(reward, list.end().winnerMiner);
    }
    return true;
}

