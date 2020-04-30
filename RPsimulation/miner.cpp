#include "miner.h"

Miner::Miner() {
    
}
Miner Miner::operator=(const Miner & orig) {
    firstName = orig.firstName;
    lastName = orig.lastName;
    idValue = orig.idValue;
    joinedTimestamp = orig.joinedTimestamp;
    reputation = orig.reputation;
    violationsCount = orig.violationsCount;
    miningPower = orig.miningPower;
    dishonestyFactor = orig.dishonestyFactor;
    roundsPlayed = orig.roundsPlayed;
    alltimeRewards = orig.alltimeRewards;
    return *this;
}

std::string Miner::getFirstName() {
    return firstName;
}

std::string Miner::getLastName() {
    return lastName;
}

long Miner::getID() {
    return idValue;
}

long Miner::getJoinedTimestamp() {
    return joinedTimestamp;
}

int Miner::getMiningPower() {
    return miningPower;
}
int& Miner::violation() {
    return violationsCount;
}

int& Miner::roundPlayed() {
    return roundsPlayed;
}

double Miner::dFact() {
    return dishonestyFactor;
}

void Miner::initialize() {
    firstName = gen.string_name();
    lastName = gen.string_name();
    idValue = gen.id_number(1010136118, 1998952326);
    joinedTimestamp = std::time(nullptr);
    reputation = 0;
    violationsCount = 0;
    miningPower = gen.minig_power();
    dishonestyFactor = gen.dishonestyFactor();
    roundsPlayed = 0;
    alltimeRewards = 0;
}

void Miner::print() {
    std::cout << "\n------------------------------------------\n";
    std::cout << "Name:\t\t" << firstName << " " << lastName << std::endl;
    std::cout << "ID No:\t\t" << idValue << std::endl;
    std::cout << "Jonied:\t\t" << std::asctime(std::localtime(&joinedTimestamp));
    std::cout << "Reward:\t\t" << alltimeRewards << std::endl;
    std::cout <<  "Reput:\t\t" << reputation << std::endl;
    std::cout << "dFact:\t\t" << dishonestyFactor << std::endl;
    std::cout << "Played:\t\t" << roundsPlayed << std::endl;
    std::cout << "------------------------------------------\n";
}
