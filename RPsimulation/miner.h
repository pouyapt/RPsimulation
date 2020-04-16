#include "STDLibraries.h"
#include "listDS.h"

#ifndef miner_h
#define miner_h

class Miner {
private:
    std::string firstName;
    std::string lastName;
    std::string idValue;
    int joinedTimestamp;
    float reputation;
    struct currentRound {
        double collectedRewards;
        int roundNumber;
        std::string miningPool;
        int violationsCount;
    };
    struct allRounds {
        double collectedRewards;
        int roundsPlayed;
    };
};


#endif
