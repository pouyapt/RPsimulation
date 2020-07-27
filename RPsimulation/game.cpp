#include "game.h"

Miner* winerMiner(MinerPopulation & DB) {
    Miner* target = NULL;
    auto n = gen.random_hash(4, DB.totalHashPower());
    auto i = gen.random_hash(0, DB.size());
    auto count = DB.size();
    unsigned sum = 0;
    while (count) {
        sum = sum + DB[i]->getMiningPower("mine");
        if (sum >= n) {
            target = DB[i];
            break;
        }
        i++;
        count--;
    }
    return target;
}

void updateMinersPowerCost(MinerPopulation & DB) {
    for (int i=0; i<DB.size(); i++) {
        float powerCost = (DB[i]->getMiningPower()/HPperPC) * MT * DB[i]->powerConRate;
        DB[i]->reduceCost(powerCost);
    }
}

void mineGame(MinerPopulation & DB) {
    updateMinersPowerCost(DB);
    Miner* winner = winerMiner(DB);
    if (winner->pool==NULL)
        winner->receiveRewards(unitPrice);
    else
        winner->pool->receiveReward(unitPrice);
    winner->mined++;
}



