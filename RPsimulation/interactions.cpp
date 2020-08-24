#include "interactions.h"


Miner* Game::winerMiner() {
    Miner* winner = NULL;
    long n = gen->random_hash(4, MP->totalHashPower());
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
    return winner;
}

void Game::updateMinersPowerCost() {
    Time time;
    time = gen->minig_time();
    for (int i=0; i<MP->size(); i++) {
        Money powerCost;
        powerCost = (*MP)[i]->powerCostPerHour * time.toHour();
        (*MP)[i]->addCost(powerCost);
        (*MP)[i]->roundsPlayed++;
        (*MP)[i]->minedTime += time;
    }
}

void Game::mine(int count) {
    Money reward;
    reward = miningP->getUnitPerNewBlock()*miningP->getUnitPrice();
    for (int i=0; i<count; i++) {
        updateMinersPowerCost();
        Miner* winner = winerMiner();
        if (winner->pool==NULL)
            winner->receivePowRewards(reward);
        else
            winner->pool->receiveReward(reward, winner);
        winner->mined++;
    }
}





