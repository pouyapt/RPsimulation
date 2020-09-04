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
    out << unitPrice.convert() << std::endl;
    out << unitsPerBlock << std::endl;
    out.close();
    std::cout << "The game data has been saved." << std::endl;
}

void Game::updateMinersPowerCost() {
    lastRoundPowerCost = 0;
    Time time;
    time = gen->minig_time();
    lastRoundDuration = time;
    for (int i=0; i<MP->size(); i++) {
        Money powerCost;
        powerCost = (*MP)[i]->powerCostPerHour * time.toHour();
        totalNetworkCosts -= powerCost;
        lastRoundPowerCost += powerCost;
        (*MP)[i]->addCost(powerCost);
        (*MP)[i]->roundsPlayed++;
        (*MP)[i]->minedTime += time;
    }
}

void Game::mine(int count) {
    Money reward;
    reward = unitPrice * unitsPerBlock;
    for (int i=0; i<count; i++) {
        updateMinersPowerCost();
        Miner* winner = winerMiner();
        if (winner->pool==NULL)
            winner->receivePowRewards(reward);
        else
            winner->pool->receiveReward(reward, winner);
        winner->mined++;
        totalNetworkRevenue += reward;
        updateVariableParameters();
        MP->updateList();
        updateUnitPrice();
    }
}

void Game::updateUnitPrice() {
    currentCostRewardRatio = sigmoidFunction(MP->size(), populationP->zeroRevenuePopulation, 0.3, 4000);
    Money aveCost;
    aveCost = lastRoundPowerCost.convert() / lastRoundDuration.convertToNumber();
    aveCost = aveCost * miningP->miningTimeMean;
    unitPrice = (aveCost.convert() / unitsPerBlock) * (currentCostRewardRatio + 1);
}

void Game::generateInitialUnitPrice() {
    double p = double(MP->size()) * gen->errorFactor(2, 0.7, true) / unitsPerBlock;
    unitPrice = p;
}




