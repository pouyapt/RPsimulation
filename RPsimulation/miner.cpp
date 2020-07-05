#include "miner.h"

Miner::Miner() {
    initialize();
}

Miner::Miner(std::string) {
    
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

std::string& Miner::getFirstName() {
    return firstName;
}

std::string& Miner::getLastName() {
    return lastName;
}

unsigned int& Miner::getID() {
    return idValue;
}

long& Miner::getJoinedTimestamp() {
    return joinedTimestamp;
}

int& Miner::getMiningPower() {
    return miningPower;
}
int& Miner::violation() {
    return violationsCount;
}

int& Miner::roundPlayed() {
    return roundsPlayed;
}

float& Miner::getReputation() {
    return reputation;
}

float& Miner::dFact() {
    return dishonestyFactor;
}

bool& Miner::isTaken() {
    return taken;
}

void Miner::initialize() {
    firstName = gen.name();
    lastName = gen.name();
    idValue = gen.id_number(1001301180, 3999952329);
    joinedTimestamp = std::time(nullptr);
    reputation = 0;
    violationsCount = 0;
    miningPower = gen.minig_power();
    dishonestyFactor = gen.dishonestyFactor();
    roundsPlayed = 0;
    alltimeRewards = 0;
    taken = false;
}

void Miner::print() {
    std::cout << "\n------------------------------------------\n";
    std::cout << "Name:\t\t" << firstName << " " << lastName << std::endl;
    std::cout << "ID No:\t\t" << idValue << std::endl;
    std::cout << "Jonied:\t\t" << std::asctime(std::localtime(&joinedTimestamp));
    std::cout << "mPower:\t\t" << miningPower << std::endl;
    std::cout << "Reward:\t\t" << alltimeRewards << std::endl;
    std::cout <<  "Reput:\t\t" << reputation << std::endl;
    std::cout << "dFact:\t\t" << dishonestyFactor << std::endl;
    std::cout << "Played:\t\t" << roundsPlayed << std::endl;
    std::cout << "Taken:\t\t" << taken << std::endl;
    std::cout << "------------------------------------------\n";
}


//----------------------------------------------------------------------------------

MinerPopulation::MinerPopulation(int population, std::string file) {
    this->file = file;
    if (readMinerPopulation())
        return;
    for (int i=0; i<population; i++) {
        list.push_back(new Miner);
        totalHashPower_+= list.end()->getMiningPower();
    }
}

MinerPopulation::~MinerPopulation() {
    writeMinerPopulation();
}


Miner* MinerPopulation::getMiner(unsigned index) {
    return list[index];
}

void MinerPopulation::updateList() {
    
}

unsigned MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

void MinerPopulation::writeMinerPopulation () {
    std::ofstream out;
    out.open(file);
    out << list.size() << std::endl;
    out << totalHashPower_ << std::endl;
    for (auto i=0; i<list.size(); i++) {
        out << list[i]->getFirstName() << std::endl;
        out << list[i]->getLastName() << std::endl;
        out << list[i]->getID() << std::endl;
        out << list[i]->getJoinedTimestamp() << std::endl;
        out << list[i]->getMiningPower() << std::endl;
        out << list[i]->dFact() << std::endl;
        out << list[i]->getReputation()<< std::endl;
    }
    out.close();
    std::cout << "The miners database have been saved to a file." << std::endl << std::endl;
}


bool MinerPopulation::readMinerPopulation () {
    std::ifstream in;
    in.open(file);
    if (in.fail())
    {
        std::cout << "The file did not open" << std::endl;
        return false;
    }
    int size;
    in >> size;
    in >> totalHashPower_;
    while (size)
    {
        Miner* newItem = new Miner("blank");
        in >> newItem->getFirstName();
        in >> newItem->getLastName();
        in >> newItem->getID();
        in >> newItem->getJoinedTimestamp();
        in >> newItem->getMiningPower();
        in >> newItem->dFact();
        in >> newItem->getReputation();
        list.push_back(newItem);
        size--;
    }
    in.close();
    return true;
}

