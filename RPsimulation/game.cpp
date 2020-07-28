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
        double powerCost = round_((double(DB[i]->getMiningPower())/double(HPperPC)) * double(MT) * double(DB[i]->powerConRate));
        DB[i]->reduceCost(powerCost);
        DB[i]->roundsPlayed++;
    }
}

void mineGame(MinerPopulation & DB) {
    updateMinersPowerCost(DB);
    Miner* winner = winerMiner(DB);
    if (winner->pool==NULL)
        winner->receiveRewards(UP);
    else
        winner->pool->receiveReward(UP);
    winner->mined++;
}



