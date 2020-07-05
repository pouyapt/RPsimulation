#include "poolManager.h"
#include "miner.h"
#include "listDS.h"

#ifndef miningPool_h
#define miningPool_h

class MiningPool {
private:
    std::string name;
    PoolManager manager;
    double rewards;
    core::list<Miner*> miners;
    
};

#endif
