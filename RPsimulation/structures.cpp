#include "structures.h"


MinerPopulation::MinerPopulation() {
    int population0 = 0;
    if (!readPopulationData()) {
        population0 = calculateInitialPopulation();
        T->createNewModulator(populationP->growthRandomnessRange, populationP->growthRandomnessRangeMinPhase, populationP->growthRandomnessRangeMaxPhase, "growthRandomness");
    }
    updateVariableParameters();
    for (auto i=0; i<population0; i++)
        addMiner();
}

MinerPopulation::~MinerPopulation() {
    writePopulationData();
    writeActiveMinersToCsvFile();
    for (auto i=0; i<allMinersList.size(); i++)
        delete allMinersList[i];
}

unsigned int MinerPopulation::size() {
    return unsigned(allMinersList.size());
}

Miner* MinerPopulation::operator [](unsigned index) {
    return allMinersList[index];
}

long MinerPopulation::totalHashPower() {
    return totalHashPower_;
}

void MinerPopulation::updateVariableParameters() {
    variableP->current.minersPopulation = allMinersList.size();
    variableP->current.totalHashPower = totalHashPower_;
    variableP->current.inactiveMinersPopulation = removdMinersCount;
}

void MinerPopulation::resetGlobalParameters() {
    populationP = &PopulationParameters::instance();
    miningP = &MiningParameters::instance();
    gen = &core::Random::instance();
    variableP = &Stats::instance();
    T = &MasterTime::instance();
}

void MinerPopulation::writeMinersData(std::ofstream &out) {
    for (auto i=0; i<allMinersList.size(); i++) {
        out << allMinersList[i]->firstName << std::endl;
        out << allMinersList[i]->lastName << std::endl;
        out << allMinersList[i]->idValue << std::endl;
        out << allMinersList[i]->joinTimestamp << std::endl;
        out << allMinersList[i]->joinedRound << std::endl;
        out << allMinersList[i]->poolFeeSensetive << std::endl;
        out << allMinersList[i]->leaveIfReachProfitTh << std::endl;
        out << allMinersList[i]->neverJoinPool << std::endl;
        out << allMinersList[i]->lastPlayedRound << std::endl;
        out << allMinersList[i]->hashPower << std::endl;
        out << allMinersList[i]->m.name << std::endl;
        out << allMinersList[i]->m.hashRate << std::endl;
        out << allMinersList[i]->m.wattage << std::endl;
        out << allMinersList[i]->powerCostPerHour.convert() << std::endl;
        out << allMinersList[i]->dishonestyFactor << std::endl;
        out << allMinersList[i]->powerConRate.convert() << std::endl;
        out << allMinersList[i]->powCount << std::endl;
        out << allMinersList[i]->poolJoined << std::endl;
        out << allMinersList[i]->minedTime.convertToNumber() << std::endl;
        out << allMinersList[i]->incomePow << std::endl;
        out << allMinersList[i]->incomePool << std::endl;
        out << allMinersList[i]->incomeDishonesty << std::endl;
        out << allMinersList[i]->poolCosts.convert() << std::endl;
        out << allMinersList[i]->soloCosts.convert() << std::endl;
        out << allMinersList[i]->BW_assigned<< std::endl;
        out << allMinersList[i]->r.reputation << std::endl;
        out << allMinersList[i]->r.defect << std::endl;
        out << allMinersList[i]->r.cycle << std::endl;
        out << allMinersList[i]->r.defectCount << std::endl;
        out << allMinersList[i]->expectedPowCount << std::endl;
        out << allMinersList[i]->BWAttackRounds << std::endl;
        out << allMinersList[i]->allBWAttacks << std::endl;
        out << allMinersList[i]->poolRemoved << std::endl;
        out << allMinersList[i]->probabilityConfidence << std::endl;
        out << allMinersList[i]->investment.convert() << std::endl;
        out << allMinersList[i]->lossTolerance.convert() << std::endl;
        out << allMinersList[i]->targetProfit.convert() << std::endl;
        out << allMinersList[i]->receivedInvitationsCount << std::endl;
        out << allMinersList[i]->newMiner << std::endl;
        out << allMinersList[i]->currentPoolCycle.rounds << std::endl;
        out << allMinersList[i]->currentPoolCycle.powCount << std::endl;
        out << allMinersList[i]->currentPoolCycle.expectedPowCount << std::endl;
    }
}

void MinerPopulation::readMinersData(std::ifstream &in, int size) {
    for (auto i=0; i<size; i++) {
        Miner* newItem = new Miner('b');
        in >> newItem->firstName;
        in >> newItem->lastName;
        in >> newItem->idValue;
        in >> newItem->joinTimestamp;
        in >> newItem->joinedRound;
        in >> newItem->poolFeeSensetive;
        in >> newItem->leaveIfReachProfitTh;
        in >> newItem->neverJoinPool;
        in >> newItem->lastPlayedRound;
        in >> newItem->hashPower;
        in >> newItem->m.name;
        in >> newItem->m.hashRate;
        in >> newItem->m.wattage;
        in >> newItem->powerCostPerHour;
        in >> newItem->dishonestyFactor;
        in >> newItem->powerConRate;
        in >> newItem->powCount;
        in >> newItem->poolJoined;
        in >> newItem->minedTime;
        in >> newItem->incomePow;
        in >> newItem->incomePool;
        in >> newItem->incomeDishonesty;
        in >> newItem->poolCosts;
        in >> newItem->soloCosts;
        in >> newItem->BW_assigned;
        in >> newItem->r.reputation;
        in >> newItem->r.defect;
        in >> newItem->r.cycle;
        in >> newItem->r.defectCount;
        in >> newItem->expectedPowCount;
        in >> newItem->BWAttackRounds;
        in >> newItem->allBWAttacks;
        in >> newItem->poolRemoved;
        in >> newItem->probabilityConfidence;
        in >> newItem->investment;
        in >> newItem->lossTolerance;
        in >> newItem->targetProfit;
        in >> newItem->receivedInvitationsCount;
        in >> newItem->newMiner;
        in >> newItem->currentPoolCycle.rounds;
        in >> newItem->currentPoolCycle.powCount;
        in >> newItem->currentPoolCycle.expectedPowCount;
        allMinersList.push_back(newItem);
    }
}

void MinerPopulation::writePopulationData () {
    std::ofstream pd;
    pd.open(populationDataFile);
    pd << allMinersList.size() << std::endl;
    pd << removdMinersCount << std::endl;
    pd << totalHashPower_ << std::endl;
    pd << population << std::endl;
    pd << populationStage << std::endl;
    pd << variableP->current.numberOfPoolMiners << std::endl;
    pd.close();
    std::ofstream md;
    md.open(minersDataFile);
    writeMinersData(md);
    md.close();
    std::cout << "The miner database has been saved." << std::endl;
}

bool MinerPopulation::readPopulationData () {
    std::ifstream pd;
    pd.open(populationDataFile);
    if (pd.fail()) {
        std::cout << "The population data file did not open." << std::endl;
        return false;
    }
    int size;
    pd >> size;
    pd >> removdMinersCount;
    pd >> totalHashPower_;
    pd >> population;
    pd >> populationStage;
    pd >> variableP->current.numberOfPoolMiners;
    pd.close();
    if (size==0)
        return false;
    std::ifstream md;
    md.open(minersDataFile);
    if (md.fail()) {
        std::cout << "The miners data file did not open." << std::endl;
        return false;
    }
    readMinersData(md, size);
    md.close();
    return true;
}

void MinerPopulation::writeMinersInvitations(Pools* P) {
    P->saveIndex();
    std::ofstream out;
    out.open("Data/invitations.db");
    for (auto i=0; i<allMinersList.size(); i++) {
        out << allMinersList[i]->invitations.size() << std::endl;
        for (auto j=0; j<allMinersList[i]->invitations.size(); j++)
            out << allMinersList[i]->invitations[j]->getIndex() << std::endl;
    }
    out.close();
}

void MinerPopulation::readMinersInvitations(Pools* P) {
    std::ifstream in;
    in.open("Data/invitations.db");
    if (in.fail()) {
        std::cout << "The miner invitations database file did not open." << std::endl;
        return;
    }
    for (auto i=0; i<allMinersList.size(); i++) {
        long size = 0;
        in >> size;
        while (size) {
            PoolManager* newP;
            int index;
            in >> index;
            newP = (*P)[index];
            allMinersList[i]->invitations.push_back(newP);
            size--;
        }
    }
    in.close();
}

void MinerPopulation::sort(std::string by) {
    allMinersList.sort(selectCompareFunc(by));
}

void MinerPopulation::shuffleList() {
    shuffleValueGen();
    allMinersList.sort(&compareShuffleValue);
}

void MinerPopulation::undoShuffleList() {
    allMinersList.sort(&compareOldIndex);
}

void MinerPopulation::printActiveMiners() {
    std::cout << "\n============== Active Miners =============\n";
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->print();
}

void MinerPopulation::printPopulationStat() {
    std::cout << "Total Hash:          \t" << totalHashPower() << std::endl;
    std::cout << "Active Miners:       \t" << allMinersList.size() << std::endl;
    std::cout << "Inactive Miners:     \t" << removdMinersCount << std::endl;
}

void MinerPopulation::addMiner(long time) {
    Miner* newMiner = new Miner;
    if (time!=0)
        newMiner->joinTimestamp = time;
    newMiner->joinedRound = variableP->current.totalMinedBlocks;
    newMiner->lastPlayedRound = variableP->current.totalMinedBlocks;
    allMinersList.push_back(newMiner);
    newMiner->index = unsigned(allMinersList.size());
    totalHashPower_ += newMiner->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::deleteMiner(unsigned index) {
    Miner* temp = allMinersList.pop(index);
    temp->quitTimestamp = T->getCurrentTime();
    temp->lastPlayedRound = variableP->current.totalMinedBlocks;
    writeRemovedMinerToCsvFile(temp);
    totalHashPower_ -= temp->getHashPower();
    removdMinersCount++;
    if (temp->isInPool())
        temp->pool->releaseMiner(temp);
    updateVariableParameters();
}

void MinerPopulation::shuffleValueGen() {
    long n = pow(allMinersList.size(), 3);
    for (auto i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->shuffleValue = gen->random_uniform_long(0, n);
        allMinersList[i]->oldIndex = i;
    }
}

void MinerPopulation::saveIndex() {
    for (int i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->index = i;
    }
}

void MinerPopulation::saveOldOrder() {
    for (auto i=0; i<allMinersList.size(); i++) {
        allMinersList[i]->oldIndex = i;
    }
}

int MinerPopulation::topHashPower() {
    int h=0;
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->hashPower > h ? h = allMinersList[i]->hashPower : false;
    return h;
}

int MinerPopulation::MinersWithAtLeastOneBlock() {
    int count=0;
    for (auto i=0; i<allMinersList.size(); i++)
        allMinersList[i]->powCount >= 1 ? count++ : false;
    return count;
}

int MinerPopulation::MinersWithProfit() {
    int count=0;
    for (auto i=0; i<allMinersList.size(); i++)
        (allMinersList[i]->poolCosts + variableP->current.unitPrice*(allMinersList[i]->incomePow+allMinersList[i]->incomePool+allMinersList[i]->incomeDishonesty)) > 0 ? count++ : false;
    return count;
}

void MinerPopulation::populationUpdate(long time, const Game& game) {
    calculateNumberOfNewMiners(time);
    processMinersAddition(time);
    processMinersRemoval();
}

void MinerPopulation::calculateNumberOfNewMiners(long time) {
    double growth = 0;
    if (populationStage==false) {
        for (auto i=1; i<time; i++) {
            growth = populationGrowthPhase1(T->getCurrentTime()+i);
            population += growth + growth*T->getModulatorValue("growthRandomness");
        }
        if (T->getCurrentTime() > populationP->halfMaximumMinersTime && growth < populationP->maxPopulationGrowth/2)
            populationStage = true;
    }
    else {
        for (auto i=1; i<time; i++) {
            growth = populationGrowthPhase2(allMinersList.size());
            population += growth + growth*T->getModulatorValue("growthRandomness");
        }
    }
}

void MinerPopulation::processMinersAddition(long time) {
    while (population > 1) {
        long joinedTime = gen->random_uniform_long(T->getCurrentTime(), T->getCurrentTime()+time);
        addMiner(joinedTime);
        population--;
    }
}

int MinerPopulation::calculateInitialPopulation() {
    long expectedP = sigmoid(T->getCurrentTime(), populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime, 0);
    return int(expectedP - allMinersList.size());
}

void MinerPopulation::processMinersRemoval() {
    if (removeLostMinersCountdown==0) {
        removeOutOfGameMiners();
        removeLostMinersCountdown = int(gen->random_uniform_long(20, 100));
    }
    else {
        removeLostMinersCountdown--;
    }
}

void MinerPopulation::removeOutOfGameMiners() {
    int i=0;
    while (i < allMinersList.size()) {
        if (allMinersList[i]->isBellowLossTolerance() || allMinersList[i]->isReachedTargetProfit())
            deleteMiner(i);
        else
            i++;
    }
}

double MinerPopulation::populationEstimate(long time) {
    return sigmoid(time, populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime, 0);
}

double MinerPopulation::populationGrowthPhase1(long time) {
    return sigmoidDeravative(time, populationP->maximumMiners, populationP->populationFunctionSteepness, populationP->halfMaximumMinersTime);
}

double MinerPopulation::populationGrowthPhase2(long population) {
    return sigmoid(population, populationP->maxPopulationGrowth, miningP->revenueFunctionSteepness, populationP->maximumMiners*miningP->revenueFunctionZeroPoint, populationP->maxPopulationGrowth/2);
}

void MinerPopulation::writeCsvHeaders(std::fstream & out) {
    out << "id,"
        << "first_name,"
        << "last_name,"
        << "hash_power,"
        << "cost_rate_KWPH,"
        << "round_before_start,"
        << "last_round,"
        << "actual_pow,"
        << "expected_pow,"
        << "pool_name,"
        << "bw_instance,"
        << "bw_all_rounds,"
        << "reputation,"
        << "pool_removed,"
        << "solo_costs_in_dollar,"
        << "pool_costs_in_dollar,"
        << "solo_income_in_crypto,"
        << "pool_income_in_crypto,"
        << "dishonest_income_in_crypto,"
        << "net_profit_in_dollar\n";
}

void MinerPopulation::writeMinerToCsvFile(std::fstream & out, Miner* miner) {
    out << miner->idValue << ","
        << miner->firstName << ","
        << miner->lastName << ","
        << miner->hashPower << ","
        << miner->powerConRate.convert() << ","
        << miner->joinedRound << ","
        << miner->lastPlayedRound << ","
        << miner->powCount << ","
        << miner->expectedPowCount << ",";
    if (miner->isInPool())
        out << miner->pool->poolName() << ",";
    else
        out << "-,";
    out << miner->BWAttackRounds << ","
        << miner->allBWAttacks << ","
        << miner->r.reputation << ","
        << miner->poolRemoved << ","
        << miner->soloCosts.convert() << ","
        << miner->poolCosts.convert() << ","
        << miner->incomePow << ","
        << miner->incomePool << ","
        << miner->incomeDishonesty << ","
        << miner->getProfitInDollar().convert() << "\n";
}

void MinerPopulation::writeRemovedMinerToCsvFile(Miner *miner) {
    if (!file_exist(inactiveMinersCsvFile)) {
        std::fstream uidlFile(inactiveMinersCsvFile, std::fstream::in | std::fstream::out | std::fstream::app);
        if (uidlFile.is_open())
            writeCsvHeaders(uidlFile);
        uidlFile.close();
    }
    std::fstream uidlFile(inactiveMinersCsvFile, std::fstream::in | std::fstream::out | std::fstream::app);
    if (uidlFile.is_open())
        writeMinerToCsvFile(uidlFile, miner);
    uidlFile.close();
}

void MinerPopulation::writeActiveMinersToCsvFile() {
    std::fstream out(activeMinersCsvFile, std::fstream::out);
    if (out.is_open()) {
        writeCsvHeaders(out);
        for (auto i=0; i<allMinersList.size(); i++)
            writeMinerToCsvFile(out, allMinersList[i]);
    }
    out.close();
}

//----------------------------------------------------------------------------------

Pools::Pools(int population) {
    if (!readPools()) {
        if (population==INT_MIN)
            makePools(populationP->defaultNumberOfPool);
        else
            makePools(population);
    }
    else
        MP->readMinersInvitations(this);
    updateVariableP();
}

Pools::~Pools() {
    MP->saveIndex();
    MP->writeMinersInvitations(this);
    writePools();
    saveActivePoolsToCsv();
    for (int i=0; i<poolList.size(); i++)
        delete poolList[i];
}

PoolManager* Pools::operator [] (unsigned index) {
    return poolList[index];
}

void Pools::makePools(int number) {
    for (int i=0; i<number; i++) {
        PoolManager* newPM = new PoolManager;
        poolList.push_back(newPM);
    }
}

PoolManager* Pools::getPool (unsigned index) {
    return poolList[index];
}

unsigned int Pools::size() {
    return unsigned(poolList.size());
}

void Pools::print() {
    for (int i=0; i<poolList.size(); i++)
        poolList[i]->print();
}

void Pools::updateVariableP() {
    variableP->current.poolsPopulation = poolList.size();
}

void Pools::writePools() {
    std::ofstream out;
    out.open(poolFile);
    out << poolList.size() << std::endl;
    for (auto i=0; i<poolList.size(); i++) {
        out << poolList[i]->index << std::endl;
        out << poolList[i]->firstName << std::endl;
        out << poolList[i]->lastName << std::endl;
        out << poolList[i]->idValue << std::endl;
        out << poolList[i]->dishonest << std::endl;
        out << poolList[i]->poolIncome << std::endl;
        out << poolList[i]->poolDishonestIncome << std::endl;
        out << poolList[i]->cost.convert() << std::endl;
        out << poolList[i]->bribePayed << std::endl;
        out << poolList[i]->costPerMiner.convert() << std::endl;
        out << poolList[i]->lossTolerance.convert() << std::endl;
        out << poolList[i]->powCount << std::endl;
        out << poolList[i]->expectedPowCount << std::endl;
        out << poolList[i]->dishonestPowCount << std::endl;
        out << poolList[i]->detectionCycle << std::endl;
        out << poolList[i]->detectionCycleRemaining << std::endl;
        out << poolList[i]->invitationProcessCycle << std::endl;
        out << poolList[i]->MiningPool::poolName() << std::endl;
        out << poolList[i]->MiningPool::poolFee << std::endl;
        out << poolList[i]->MiningPool::powReward << std::endl;
        out << poolList[i]->totalHashPower << std::endl;
        out << poolList[i]->sendInvitationsCount << std::endl;
        out << poolList[i]->size() << std::endl;
        for (auto j=0; j<poolList[i]->size(); j++) {
            out << poolList[i]->getMiner(j)->getIndex() << std::endl;
        }
    }
    out.close();
    std::cout << "The pool database have been saved." << std::endl;
}

bool Pools::readPools () {
    std::ifstream in;
    in.open(poolFile);
    if (in.fail())
    {
        std::cout << "The pool database file did not open." << std::endl;
        return false;
    }
    int sizeAll;
    int sizeMiners;
    int index;
    in >> sizeAll;
    while (sizeAll) {
        PoolManager* newPM = new PoolManager("blank");
        in >> newPM->index;
        in >> newPM->firstName;
        in >> newPM->lastName;
        in >> newPM->idValue;
        in >> newPM->dishonest;
        in >> newPM->poolIncome;
        in >> newPM->poolDishonestIncome;
        in >> newPM->cost;
        in >> newPM->bribePayed;
        in >> newPM->costPerMiner;
        in >> newPM->lossTolerance;
        in >> newPM->powCount;
        in >> newPM->expectedPowCount;
        in >> newPM->dishonestPowCount;
        in >> newPM->detectionCycle;
        in >> newPM->detectionCycleRemaining;
        in >> newPM->invitationProcessCycle;
        in >> newPM->MiningPool::poolName();
        in >> newPM->MiningPool::poolFee;
        in >> newPM->MiningPool::powReward;
        in >> newPM->totalHashPower;
        in >> newPM->sendInvitationsCount;
        in >> sizeMiners;
        while (sizeMiners) {
            in >> index;
            newPM->pushBack((*MP)[index]);
            (*MP)[index]->savePoolManager(newPM);
            sizeMiners--;
        }
        poolList.push_back(newPM);
        sizeAll--;
    }
    in.close();
    return true;
}

void Pools::shuffle() {
    long n = pow(poolList.size(), 3);
    for (auto i=0; i<poolList.size(); i++)
        poolList[i]->index = gen->random_uniform_long(0, n);
    poolList.sort(&compareIndex);
}

void Pools::saveIndex() {
    for (auto i=0; i<poolList.size(); i++)
        poolList[i]->index = i;
}

void Pools::removeLostPools() {
    for (auto i=0; i<poolList.size(); i++) {
        if (poolList[i]->isBellowLossTolerance()) {
            for (int j=0; j<BW->list.size(); j++) {
                if (poolList[i] == BW->list[j].suspect || poolList[i] == BW->list[j].victim)
                    BW->list.pop(j);
            }
            poolList[i]->releaseAllMiners();
            std::cout << "pool deleted" << std::endl;
            poolList.pop(i);
            saveIndex();
        }
    }
}

void Pools::saveActivePoolsToCsv() {
    std::fstream out(activePoolsCsvFile, std::fstream::out);
    if (out.is_open()) {
        writeCsvHeaders(out);
        for (auto i=0; i<poolList.size(); i++)
            writePoolToCsvFile(out, poolList[i]);
    }
    out.close();
}

void Pools::writeCsvHeaders(std::fstream & out) {
    out << "id,"
        << "pool_name,"
        << "pool_fee_rate,"
        << "pow_reward_rate,"
        << "hash_power,"
        << "hash_power_share,"
        << "miners_count,"
        << "honest_pow,"
        << "dishonest_pow,"
        << "expected_pow,"
        << "costs_in_dollar,"
        << "honest_income_in_crypto,"
        << "dishonet_income_in_crypto,"
        << "net_profit_in_dollar\n";
}

void Pools::writePoolToCsvFile(std::fstream & out, PoolManager* p) {
    out << p->idValue << ","
        << p->name << ","
        << p->poolFee() << ","
        << p->powReward << ","
        << p->totalHashPower << ","
        << double(p->totalHashPower)/double(variableP->getCurrentTotalHashPower()) << ","
        << p->size() << ","
        << p->powCount << ","
        << p->dishonestPowCount << ","
        << p->expectedPowCount << ","
        << p->cost.convert() << ","
        << p->poolIncome << ","
        << p->poolDishonestIncome << ","
        << p->cost.convert()+(variableP->getUnitPrice().convert()*(p->poolIncome+p->poolDishonestIncome)) << "\n";
}
