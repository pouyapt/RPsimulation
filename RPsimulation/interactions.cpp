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
        int n = int(gen->random_uniform_long(MP->size()/15, MP->size()/5));
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



