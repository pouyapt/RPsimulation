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
    unsigned long last;
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

void printStats();
void saveStats_csv();

#endif /* foundation_h */
