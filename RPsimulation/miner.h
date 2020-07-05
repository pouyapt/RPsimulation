#include "generators.h"
#include "listDS.h"

#ifndef miner_h
#define miner_h

class Miner {
private:
    std::string firstName;
    std::string lastName;
    unsigned int idValue;
    std::time_t joinedTimestamp;
    float reputation;
    int miningPower;
    int violationsCount;
    float alltimeRewards;
    int roundsPlayed;
    float dishonestyFactor;
    bool taken;
    core::random gen;
    void initialize();
public:
    Miner();
    Miner(std::string);
    Miner operator=(const Miner & orig);
    std::string& getFirstName();
    std::string& getLastName();
    unsigned int& getID();
    long& getJoinedTimestamp();
    float& getReputation();
    int& getMiningPower();
    int& violation();
    int& roundPlayed();
    float& dFact();
    bool& isTaken();
    void print();
};

//--------------------------------------------------------------------------------

class MinerPopulation {
private:
    core::list<Miner*> list;
    unsigned int totalHashPower_;
    std::string file;
    void writeMinerPopulation ();
    bool readMinerPopulation ();
public:
    MinerPopulation(int population=15000, std::string file="minersList.txt");
    ~MinerPopulation();
    void updateList();
    Miner* getMiner (unsigned index);
    unsigned totalHashPower();
};


#endif
