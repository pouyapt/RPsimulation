#include "interactions.h"


Game::Game() {
    file = "Data/game.db";
    if (!ReadGameFile()) {
        totalNetworkRevenue = 0;
        totalNetworkCosts = 0;
        generateInitialUnitPrice();
    }
    updateVariableParameters();
}

Game::~Game() {
    WriteGameFile();
}

void Game::updateVariableParameters() {
    variableP->totalRevenue = totalNetworkRevenue.convert();
    variableP->totalCost = totalNetworkCosts.convert();
    variableP->totalMinedBlocks = totalMinedBlocks;
    variableP->unitPrice = unitPrice.convert();
    variableP->unitPerNewBlock = unitsPerBlock;
    variableP->currentCostRewardRatio = currentCostRewardRatio;
    variableP->lastGeneratedBlockTime = lastGeneratedBlockTimestamp.convertToNumber();
}

Miner* Game::winerMiner() {
    Miner* winner = NULL;
    long n = gen->random_uniform_long(4, MP->totalHashPower());
    unsigned i = gen->select_random_index(0, MP->size());
    auto count = MP->size();
    long sum = 0;
    while (count) {
        sum = sum + (*MP)[i]->getMiningPower("mine");
        if (sum >= n) {
            winner = (*MP)[i];
            break;
        }
        i++;
        count--;
    }
    totalMinedBlocks++;
    return winner;
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
    in >> unitsPerBlock;
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
    out << lastGeneratedBlockTimestamp << std::endl;
    out << unitPrice.convert() << std::endl;
    out << unitsPerBlock << std::endl;
    out.close();
    std::cout << "The game data has been saved." << std::endl;
}

void Game::updateMinersPowerCost() {
    lastRoundPowerCost = 0;
    Time time;
    time = gen->minig_time();
    lastGeneratedBlockTimestamp = time + T->getCurrentTime();
    MP->updatePopulation(time.convertToNumber());
    lastRoundDuration = time;
    for (int i=0; i<MP->size(); i++) {
        Money powerCost;
        Time minedDuration;
        if ((*MP)[i]->joinedTimestamp > T->getCurrentTime())
            minedDuration = lastGeneratedBlockTimestamp - (*MP)[i]->joinedTimestamp;
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

void Game::mine(int count) {
    Money reward;
    for (int i=0; i<count; i++) {
        reward = unitPrice * unitsPerBlock;
        updateMinersPowerCost();
        Miner* winner = winerMiner();
        if (winner->pool==NULL)
            winner->receivePowRewards(reward);
        else
            winner->pool->receiveReward(reward, winner);
        winner->mined++;
        totalNetworkRevenue += reward;
        updateVariableParameters();
        saveStats_csv();
        MP->updateLosingMiners();
        updateUnitPrice();
    }
}

void Game::updateUnitPrice() {
    currentCostRewardRatio = sigmoid(MP->size(), miningP->revenueFactor, miningP->revenueFunctionSteepness, populationP->minersCarryingCapacity*miningP->revenueFunctionZeroPoint, miningP->revenueFactor/2);
    Money aveCost;
    aveCost = lastRoundPowerCost.convert() / lastRoundDuration.convertToNumber();
    aveCost = aveCost * miningP->miningTimeMean;
    unitPrice = (aveCost.convert() / unitsPerBlock) * (currentCostRewardRatio + 1);
}

void Game::generateInitialUnitPrice() {
    double p = double(MP->size()) * gen->errorFactor(2, 0.7, true) / unitsPerBlock;
    unitPrice = p;
}

Time Game::lastGeneratedBlockTime() const {
    return lastGeneratedBlockTimestamp;
}




