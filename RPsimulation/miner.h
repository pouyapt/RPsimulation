#include "coreDataType.h"
#include "listDS.h"
#include "generators.h"

#ifndef miner_h
#define miner_h

class Miner {
private:
    std::string firstName;
    std::string lastName;
    long idValue;
    std::time_t joinedTimestamp;
    float reputation;
    int miningPower;
    int violationsCount;
    double alltimeRewards;
    int roundsPlayed;
    double dishonestyFactor;
    core::random gen;
public:
    Miner();
    Miner operator=(const Miner & orig);
    std::string getFirstName();
    std::string getLastName();
    long getID();
    long getJoinedTimestamp();
    float getReputation();
    int getMiningPower();
    int& violation();
    int& roundPlayed();
    double dFact();
    void initialize();
    void print();
};
    

#endif
