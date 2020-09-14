#ifndef BaseUnits_h
#define BaseUnits_h

#include "listDS.h"

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

class Stats {
public:
    static Stats& instance() {
        static Stats instance;
        return instance;
    }
    Stats operator=(const Stats & orig) = delete;
    friend class MinerPopulation;
    friend class Pools;
    friend class Game;
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
private:
    Stats() {}
    ~Stats();
    struct snapShot {
        long time = 0;
        Money unitPrice = 0;
        double unitPerNewBlock = 0;
        long totalHashPower = 0;
        long minersPopulation = 0;
        long inactiveMinersPopulation = 0;
        long poolsPopulation = 0;
        double costRewardRatio = 0;
        int numberOfPoolMiners = 0;
        int MinersWithAtLeastOneBlock = 0;
        int highestMinerReputation = 0;
        int lowestMinerReputation = 0;
        int numberOfAllViolations = 0;
        int numberOfDetectedViolations = 0;
        Money totalRevenue = 0;
        Money totalCost = 0;
        int totalMinedBlocks = 0;
        long lastGeneratedBlockTime = 0;
    };
    snapShot current;
    std::vector<snapShot> snapShots;
};

//----------------------------------------------------------------------------------

#endif /* foundation_h */
