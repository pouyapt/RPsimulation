#ifndef BaseUnits_h
#define BaseUnits_h

#include "listDS.h"

struct TrustBData {
    double reputation = 0;
    double defect = 0;
    int cycle = 0;
    int defectCount = 0;
};

class TrustA {
private:
    TrustAParameters* p = &TrustAParameters::instance();
    double mu_x_B(double x);
    double mu_x_N(double x);
    double mu_x_G(double x);
    double mu_x_1(double x);
    double mu_prime_x_B(double x);
    double mu_prime_x_N(double x);
    double mu_prime_x_G(double x);
    double mu_prime_x_1(double x);
public:
    double cooperation(double x);
    double defection(double x);
};

class TrustB {
private:
    TrustBParameters* p = &TrustBParameters::instance();
public:
    void cooperation(TrustBData & t);
    void defection(TrustBData & t);
};

//----------------------------------------------------------------------------------

struct AttackCase {
    AttackCase() {
        victim = nullptr;
        suspect = nullptr;
        bribedMiner = nullptr;
        bribePercentage = 0;
        rounds = 0;
        remainingRounds = 0;
    }
    PoolManager* victim;
    PoolManager* suspect;
    Miner* bribedMiner;
    double bribePercentage;
    int rounds;
    int remainingRounds;
};

//---------------------------------------------------------------------------------

class BW_Attack_Data {
private:
    core::list<AttackCase> list;
public:
    static BW_Attack_Data& instance() {
        static BW_Attack_Data instance;
        return instance;
    }
    friend class MinerPopulation;
    friend class Pools;
    friend class BW_Attack;
    friend class PoolManager;
};

//----------------------------------------------------------------------------------

class Time {
private:
    int second;
    int minute;
    long hour;
    void convertToTime(long seconds);
    std::string print();
public:
    long convertToNumber() const;
    double toHour();
    Time();
    Time operator=(const long &value);
    Time operator=(const int &value);
    Time operator+(const Time &next);
    Time operator+(const long &next);
    Time operator+(const int &next);
    Time operator-(const Time &next);
    Time operator-(const long &next);
    Time operator-(const int &next);
    Time operator*(const Time &next);
    Time operator*(const long &next);
    Time operator*(const int &next);
    Time operator/(const Time &next);
    Time operator/(const long &next);
    Time operator/(const int &next);
    Time& operator+=(const Time &value);
    Time& operator+=(const long &value);
    Time& operator+=(const int &value);
    Time& operator-=(const Time &value);
    Time& operator-=(const long &value);
    Time& operator-=(const int &value);
    bool operator>(const Time& m);
    bool operator<(const Time& m);
    bool operator>=(const Time& m);
    bool operator<=(const Time& m);
    friend std::ostream& operator<<(std::ostream& os, Time& dl);
    friend std::istream& operator>>(std::istream& os, Time& dl);
};

//----------------------------------------------------------------------------------

class VirtualTime {
private:
    unsigned long last = 0;
    MiningParameters* miningP = &MiningParameters::instance();
    void writeTime();
    bool readTime();
    VirtualTime();
    ~VirtualTime();
public:
    static VirtualTime& instance() {
        static VirtualTime instance;
        return instance;
    }
    long getCurrentTime();
    long addSecondsToCurrentTime(long seconds);
    long addHoursToCurrentTime(double hours);
    friend class MasterTime;
};


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
    Money operator=(const long &&value);
    Money operator=(const int &value);
    Money operator=(const int &&value);
    Money operator=(const double &value);
    Money operator=(const double &&value);
    Money operator+(const Money &next);
    Money operator+(const Money &&next);
    Money operator+(const long &value);
    Money operator+(const long &&value);
    Money operator+(const int &value);
    Money operator+(const int &&value);
    Money operator+(const double &value);
    Money operator+(const double &&value);
    Money operator-(const Money &next);
    Money operator-(const Money &&next);
    Money operator-(const long &value);
    Money operator-(const long &&value);
    Money operator-(const int &value);
    Money operator-(const int &&value);
    Money operator-(const double &value);
    Money operator-(const double &&value);
    Money operator*(const Money &next);
    Money operator*(const Money &&next);
    Money operator*(const long &value);
    Money operator*(const long &&value);
    Money operator*(const int &value);
    Money operator*(const int &&value);
    Money operator*(const double &value);
    Money operator*(const double &&value);
    Money operator/(const Money &next);
    Money operator/(const Money &&next);
    Money operator/(const long &value);
    Money operator/(const long &&value);
    Money operator/(const int &value);
    Money operator/(const int &&value);
    Money operator/(const double &value);
    Money operator/(const double &&value);
    Money& operator+=(const Money &value);
    Money& operator+=(const Money &&value);
    Money& operator+=(const double &value);
    Money& operator+=(const double &&value);
    Money& operator+=(const long &value);
    Money& operator+=(const long &&value);
    Money& operator+=(const int &value);
    Money& operator+=(const int &&value);
    Money& operator-=(const Money &value);
    Money& operator-=(const Money &&value);
    Money& operator-=(const double &value);
    Money& operator-=(const double &&value);
    Money& operator-=(const long &value);
    Money& operator-=(const long &&value);
    Money& operator-=(const int &value);
    Money& operator-=(const int &&value);
    bool operator>(const Money& m);
    bool operator>(const Money&& m);
    bool operator<(const Money& m);
    bool operator<(const Money&& m);
    bool operator>=(const Money& m);
    bool operator>=(const Money&& m);
    bool operator<=(const Money& m);
    bool operator<=(const Money&& m);
    friend std::ostream& operator<<(std::ostream& os, Money& dl);
    friend std::ostream& operator<<(std::ostream& os, Money&& dl);
    friend std::istream& operator>>(std::istream& os, Money& dl);
    friend std::istream& operator>>(std::istream& os, Money&& dl);
};

//----------------------------------------------------------------------------------

class Stats {
public:
    static Stats& instance() {
        static Stats instance;
        return instance;
    }
    Stats operator=(const Stats & orig) = delete;
    std::string filename = "output/stat_snapshots.csv";
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
    friend class BW_Attack;
    friend class Reputation;
    VirtualTime* T = &VirtualTime::instance();
    void printCurrentStats();
    void saveSnapShot();
    void addNewStatsToCsvFile();
    long getCurrentTotalHashPower();
    long getCurentMinersPopulation();
    long getCurrentPoolsPopulation();
    Money getUnitPrice();
    double getUnitPerNewBlock();
    void updateNumberOfPoolMiners(int i);
    int getDishonestActivityCount();
    int getDetectedDishonestActivityCount();
    bool& inReputationMode();
private:
    Stats() {}
    ~Stats();
    bool reputationMode = true;
    void statFileInit();
    struct snapShot {
        long time = 0;
        Money unitPrice = 0;
        double unitPerNewBlock = 0;
        std::string minedBy;
        std::string pool;
        long totalHashPower = 0;
        long minersPopulation = 0;
        long inactiveMinersPopulation = 0;
        long poolsPopulation = 0;
        double costRewardRatio = 0;
        int numberOfPoolMiners = 0;
        int MinersWithAtLeastOneBlock = 0;
        int dishonestActivityCount = 0;
        int detectedDishonestActivityCount = 0;
        int falseDetectedDishonestActivityCount = 0;
        Money totalCost = 0;
        int totalMinedBlocks = 0;
        long lastGeneratedBlockTime = 0;
    };
    snapShot current;
    std::vector<snapShot> snapShots;
};

//----------------------------------------------------------------------------------

#endif /* foundation_h */
