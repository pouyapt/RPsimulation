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

int MinerPopulation::getAllViolationsCount() {
    unsigned int sum = 0;
    for (auto i=0; i<allMinersList.size(); i++) {
        sum += allMinersList[i]->allViolations;
    }
    return sum;
}

void MinerPopulation::updateVariableParameters() {
    variableP->current.minersPopulation = allMinersList.size();
    variableP->current.totalHashPower = totalHashPower_;
    variableP->current.inactiveMinersPopulation = removdMinersCount;
    variableP->current.highestMinerReputation = highestMinerReutation;
    variableP->current.lowestMinerReputation = lowestMinerReutation;
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
        out << allMinersList[i]->leftRound << std::endl;
        out << allMinersList[i]->hashPower << std::endl;
        out << allMinersList[i]->m.name << std::endl;
        out << allMinersList[i]->m.hashRate << std::endl;
        out << allMinersList[i]->m.wattage << std::endl;
        out << allMinersList[i]->powerCostPerHour.convert() << std::endl;
        out << allMinersList[i]->dishonestyFactor << std::endl;
        out << allMinersList[i]->powerConRate.convert() << std::endl;
        out << allMinersList[i]->mined << std::endl;
        out << allMinersList[i]->poolJoined << std::endl;
        out << allMinersList[i]->minedTime.convertToNumber() << std::endl;
        out << allMinersList[i]->incomePow << std::endl;
        out << allMinersList[i]->incomePool << std::endl;
        out << allMinersList[i]->incomeDishonesty << std::endl;
        out << allMinersList[i]->costs.convert() << std::endl;
        out << allMinersList[i]->BW_assigned<< std::endl;
        out << allMinersList[i]->reputation << std::endl;
        out << allMinersList[i]->reputationTimeOffset << std::endl;
        out << allMinersList[i]->violationTimeOffset << std::endl;
        out << allMinersList[i]->roundsPlayed << std::endl;
        out << allMinersList[i]->detectedViolations << std::endl;
        out << allMinersList[i]->allViolations << std::endl;
        out << allMinersList[i]->probabilityConfidence << std::endl;
        out << allMinersList[i]->investment.convert() << std::endl;
        out << allMinersList[i]->lossTolerance.convert() << std::endl;
        out << allMinersList[i]->targetProfit.convert() << std::endl;
        out << allMinersList[i]->receivedInvitationsCount << std::endl;
        out << allMinersList[i]->newMiner << std::endl;
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
        in >> newItem->leftRound;
        in >> newItem->hashPower;
        in >> newItem->m.name;
        in >> newItem->m.hashRate;
        in >> newItem->m.wattage;
        in >> newItem->powerCostPerHour;
        in >> newItem->dishonestyFactor;
        in >> newItem->powerConRate;
        in >> newItem->mined;
        in >> newItem->poolJoined;
        in >> newItem->minedTime;
        in >> newItem->incomePow;
        in >> newItem->incomePool;
        in >> newItem->incomeDishonesty;
        in >> newItem->costs;
        in >> newItem->BW_assigned;
        in >> newItem->reputation;
        in >> newItem->reputationTimeOffset;
        in >> newItem->violationTimeOffset;
        in >> newItem->roundsPlayed;
        in >> newItem->detectedViolations;
        in >> newItem->allViolations;
        in >> newItem->probabilityConfidence;
        in >> newItem->investment;
        in >> newItem->lossTolerance;
        in >> newItem->targetProfit;
        in >> newItem->receivedInvitationsCount;
        in >> newItem->newMiner;
        allMinersList.push_back(newItem);
    }
}

void MinerPopulation::writePopulationData () {
    std::ofstream pd;
    pd.open(populationDataFile);
    pd << allMinersList.size() << std::endl;
    pd << removdMinersCount << std::endl;
    pd << totalHashPower_ << std::endl;
    pd << highestMinerReutation << std::endl;
    pd << lowestMinerReutation << std::endl;
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
    pd >> highestMinerReutation;
    pd >> lowestMinerReutation;
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
        for (auto j=0; j<allMinersList[i]->invitations.size(); j++) {
            out << allMinersList[i]->invitations[j].PM->getIndex() << std::endl;
            out << allMinersList[i]->invitations[j].EstimatedDailyProfit.convert() << std::endl;
        }
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
            poolEvaluation PE;
            int index;
            in >> index;
            PE.PM = (*P)[index];
            in >> PE.EstimatedDailyProfit;
            allMinersList[i]->invitations.push_back(PE);
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
    newMiner->joinedRound = variableP->current.totalMinedBlocks+1;
    allMinersList.push_back(newMiner);
    newMiner->index = unsigned(allMinersList.size());
    totalHashPower_ += newMiner->getHashPower();
    updateVariableParameters();
}

void MinerPopulation::deleteMiner(unsigned index) {
    if (allMinersList[index]->isInPool())
        removeMinerFromPool(allMinersList[index]);
    Miner* temp = allMinersList.pop(index);
    temp->quitTimestamp = T->getCurrentTime();
    temp->leftRound = variableP->current.totalMinedBlocks;
    totalHashPower_ -= temp->getHashPower();
    writeRemovedMinerToCsvFile(temp);
    removdMinersCount++;
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
        allMinersList[i]->mined >= 1 ? count++ : false;
    return count;
}

int MinerPopulation::MinersWithProfit() {
    int count=0;
    for (auto i=0; i<allMinersList.size(); i++)
        (allMinersList[i]->costs + variableP->current.unitPrice*(allMinersList[i]->incomePow+allMinersList[i]->incomePool+allMinersList[i]->incomeDishonesty)) > 0 ? count++ : false;
    return count;
}

void MinerPopulation::populationUpdate(long time, const Game& game) {
    calculateNumberOfNewMiners(time);
    processMinersAddition(time);
    processMinersRemoval();
    updateAllReputations();
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
        removeLostMiners();
        removeLostMinersCountdown = int(gen->random_uniform_long(20, 100));
    }
    else {
        removeLostMinersCountdown--;
    }
}

void MinerPopulation::removeLostMiners() {
    int i=0;
    while (i < allMinersList.size()) {
        if (allMinersList[i]->isBellowLossTolerance())
            deleteMiner(i);
        else
            i++;
    }
}

void MinerPopulation::removeMinerFromPool(Miner* miner) {
    for (int i=0; i<BW->list.size(); i++) {
        if (BW->list[i].bribedMiner == miner)
            BW->list.pop(i);
    }
    miner->pool->releaseMiner(miner);
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

void MinerPopulation::updateAllReputations() {
    for (auto i=0; i<allMinersList.size(); i++)
        updateReputation(allMinersList[i]);
}

void MinerPopulation::updateReputation(Miner* miner) {
    double minersMembershipDuration = minerPresenceDurationInYear(miner);
    miner->reputation = sigmoid(minersMembershipDuration, 2, 1, miner->reputationTimeOffset, 1);
    updateHighestLowestReputation(miner);
}

double MinerPopulation::minerPresenceDurationInYear(Miner* miner) {
    return (T->getCurrentTime() - miner->joinTimestamp) / 31536000.0;
}

void MinerPopulation::applyNegativeReputation(Miner* miner) {
    double minersMembershipDuration = minerPresenceDurationInYear(miner);
    miner->reputationTimeOffset += minersMembershipDuration - log10(minersMembershipDuration);
    updateReputation(miner);
}

void MinerPopulation::updateHighestLowestReputation(Miner* miner) {
    if (miner->reputation > highestMinerReutation) {
        highestMinerReutation = miner->reputation;
        variableP->current.highestMinerReputation = highestMinerReutation;
        return;
    }
    if (miner->reputation < lowestMinerReutation) {
        lowestMinerReutation = miner->reputation;
        variableP->current.lowestMinerReputation = lowestMinerReutation;
        return;
    }
}

void MinerPopulation::writeCsvHeaders(std::fstream & out) {
    out << "id" << "," << "first_name" << "," << "last_name" << "," << "hash_power" << "," << "cost_rate_KWPH" << "," << "loss_tolerance"<< "," << "target_profit" << "," << "first_round" << "," << "last_round" << "," << "round_count" << "," << "mined_time" << "," << "pow_count" << "," << "dishonest_activities" << "," << "reputation" << "," << "costs_in_dollar" << "," << "solo_income_in_crypto" << "," << "pool_income_in_crypto" << "," << "dishonest_income_in_crypto" << "," << "net_profit_in_dollar" << std::endl;
}

void MinerPopulation::writeMinerToCsvFile(std::fstream & out, Miner* miner) {
    out << miner->idValue << "," << miner->firstName << "," << miner->lastName << "," << miner->hashPower << "," << miner->powerConRate.convert() << "," << miner->lossTolerance.convert() << "," << miner->targetProfit.convert() << "," << miner->joinedRound << "," << miner->leftRound << "," << miner->roundsPlayed << "," << miner->minedTime.convertToNumber() << "," << miner->mined << "," << miner->allViolations << "," << miner->reputation << "," << miner->costs.convert() << "," << miner->incomePow << "," << miner->incomePool << "," << miner->incomeDishonesty << "," << (variableP->current.unitPrice*(miner->incomePow+miner->incomePool+miner->incomeDishonesty)).convert() + miner->costs.convert() << std::endl;
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
        out << poolList[i]->minimumMembershipTime << std::endl;
        out << poolList[i]->poolIncome << std::endl;
        out << poolList[i]->cost.convert() << std::endl;
        out << poolList[i]->bribePayed << std::endl;
        out << poolList[i]->costPerMiner.convert() << std::endl;
        out << poolList[i]->lossTolerance.convert() << std::endl;
        out << poolList[i]->mined << std::endl;
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
        in >> newPM->minimumMembershipTime;
        in >> newPM->poolIncome;
        in >> newPM->cost;
        in >> newPM->bribePayed;
        in >> newPM->costPerMiner;
        in >> newPM->lossTolerance;
        in >> newPM->mined;
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
    out << "id" << "," << "pool_name" << "," << "pool_fee_rate" << "," << "pow_reward_rate" << "," << "loss_tolerance" << "," << "hash_power" << "," << "hash_power_share" << "," << "pow_count" << "," << "normal_costs" << "," << "dishonest_costs" << "," << "income" << "," << "net_profit" << std::endl;
}

void Pools::writePoolToCsvFile(std::fstream & out, PoolManager* p) {
    out << p->idValue << "," << p->name << "," << p->poolFee() << "," << p->powReward << "," << p->lossTolerance.convert() << "," << p->totalHashPower << "," << double(p->totalHashPower)/double(variableP->getCurrentTotalHashPower()) << "," << p->mined << "," << p->cost.convert() << "," << p->bribePayed << "," << p->poolIncome << "," << p->cost.convert()+(variableP->getUnitPrice().convert()*(p->bribePayed+p->poolIncome)) << std::endl;
}
