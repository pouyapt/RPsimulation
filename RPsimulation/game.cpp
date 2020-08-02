#include "game.h"


Game::Game(MinerPopulation & mainDB) {
    DB = &mainDB;
}


Miner* Game::winerMiner() {
    Miner* target = NULL;
    auto n = gen.random_hash(4, DB->totalHashPower());
    auto i = gen.random_hash(0, DB->size());
    auto count = DB->size();
    unsigned sum = 0;
    while (count) {
        sum = sum + (*DB)[i]->getMiningPower("mine");
        if (sum >= n) {
            target = (*DB)[i];
            break;
        }
        i++;
        count--;
    }
    return target;
}

void Game::updateMinersPowerCost() {
    for (int i=0; i<DB->size(); i++) {
        Money powerCost;
        powerCost = (*DB)[i]->powerCostPerHour * miningParameters.getAveMiningTime();
        (*DB)[i]->addCost(powerCost);
        (*DB)[i]->roundsPlayed++;
    }
}

void Game::mine(int count) {
    Money reward;
    reward = miningParameters.getUnitPerNewBlock()*miningParameters.getUnitPrice();
    for (int i=0; i<count; i++) {
        updateMinersPowerCost();
        Miner* winner = winerMiner();
        if (winner->pool==NULL)
            winner->receiveRewards(reward);
        else
            winner->pool->receiveReward(reward);
        winner->mined++;
    }
}





