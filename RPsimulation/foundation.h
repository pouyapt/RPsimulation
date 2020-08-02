#ifndef foundation_h
#define foundation_h

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include "listDS.h"

class random;
class Miner;
class PoolManager;
class MinerPopulation;
class Pools;
class Game;

bool is_file_exist(const char *fileName);
void databaseInit();
double round_(double var);

//----------------------------------------------------------------------------------

class EntityParameters {
private:
    int miningPowerMean;
    int miningPowerStd;
    int miningPowerMin;
    int miningPowerLowRange;
    int dishonestyFactorMean;
    int dishonestyFactorStd;
    int dishonestyFactorMin;
    int minersPopulationGrowthMean;
    int minersPopulationGrowthStd;
    int minersPopulationGrowthMin;
    double powerConsumptionMean;
    double powerConsumptionStd;
    double powerConsumptionMin;
    double powerConsumptionMax;
    double poolFeeMin;
    double poolFeeMax;
public:
    EntityParameters();
    int getMiningPowerPars(std::string parameter);
    int getDishonestyFactorPars(std::string parameter);
    int getMinersPopulationGrowthPars(std::string parameter);
    double getPowerConsumptionPars(std::string parameter);
    double getPoolFeesPars(std::string parameter);
};

//----------------------------------------------------------------------------------

static class MiningParameters {
private:
    double unitPrice;           // The Price of a Single Currency Unit
    double unitPerNewBlock;     // Number of Reward Units for a New Block
    double aveMiningTime;       // Average mining time for a New Block
public:
    MiningParameters();
    double getUnitPrice();
    double getUnitPerNewBlock();
    double getAveMiningTime();
} miningParameters;

//----------------------------------------------------------------------------------

static class PopulationParameters {
private:
    unsigned defaultMinersPopulation;
    unsigned defaultNumberOfPool;
public:
    PopulationParameters();
    friend class MinerPopulation;
    friend class Pools;
} populationParameters;

//----------------------------------------------------------------------------------

class Money {
private:
    long wholeValue;
    long fracValue;
    int precision;
    long multiplier;
    std::string addSeperator(std::string input);
    double roundDecimal(double var, int p);
    Money convertFraction(double value);
    Money convertWhole(long value);
public:
    Money(unsigned p=6);
    double convert() const;
    std::string print();
    Money operator=(const long &value);
    Money operator=(const int &value);
    Money operator=(const double &value);
    Money operator+(const Money &next);
    Money operator+(const long &value);
    Money operator+(const int &value);
    Money operator+(const double &value);
    Money operator-(const Money &next);
    Money operator-(const long &value);
    Money operator-(const int &value);
    Money operator-(const double &value);
    Money operator*(const Money &next);
    Money operator*(const long &value);
    Money operator*(const int &value);
    Money operator*(const double &value);
    Money operator/(const Money &next);
    Money operator/(const long &value);
    Money operator/(const int &value);
    Money operator/(const double &value);
    Money& operator+=(const Money &value);
    Money& operator+=(const double &value);
    Money& operator+=(const long &value);
    Money& operator+=(const int &value);
    Money& operator-=(const Money &value);
    Money& operator-=(const double &value);
    Money& operator-=(const long &value);
    Money& operator-=(const int &value);
    bool operator>(const Money& m);
    bool operator<(const Money& m);
    bool operator>=(const Money& m);
    bool operator<=(const Money& m);
    friend std::ostream& operator<<(std::ostream& os, Money& dl);
    friend std::istream& operator>>(std::istream& os, Money& dl);
};

//----------------------------------------------------------------------------------

#endif /* foundation_h */
