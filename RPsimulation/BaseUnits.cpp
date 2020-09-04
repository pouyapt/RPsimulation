#include "BaseUnits.h"


Time::Time() {
    second = 0;
    minute = 0;
    hour = 0;
}

void Time::convertToTime(long seconds) {
    hour = seconds / 3600;
    seconds = seconds - hour * 3600;
    minute = int(seconds) / 60;
    seconds = seconds - minute * 60;
    second = int(seconds);
}

long Time::convertToNumber() const {
    return (hour*3600 + minute*60 + second);
}

double Time::toHour() {
    double n = (hour*3600 + minute*60 + second);
    n /= 3600.0;
    return n;
}

std::string Time::print() {
    char result[40];
    if (second<0 || minute<0 || hour<0)
        sprintf(result, "-%ld:%.2d:%.2d", abs(hour),abs(minute),abs(second));
    else
        sprintf(result, "%ld:%.2d:%.2d", abs(hour),abs(minute),abs(second));
    return result;
}

Time Time::operator=(const long &value) {
    convertToTime(value);
    return *this;
}

Time Time::operator=(const int &value) {
    convertToTime(value);
    return *this;
}

Time Time::operator+(const Time &next) {
    long temp = convertToNumber() + next.convertToNumber();
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator+(const long &next) {
    long temp = convertToNumber() + next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator+(const int &next) {
    long temp = convertToNumber() + next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator-(const Time &next) {
    long temp = convertToNumber() - next.convertToNumber();
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator-(const long &next) {
    long temp = convertToNumber() - next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator-(const int &next) {
    long temp = convertToNumber() - next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator*(const Time &next) {
    long temp = convertToNumber() * next.convertToNumber();
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator*(const long &next) {
    long temp = convertToNumber() * next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator*(const int &next) {
    long temp = convertToNumber() * next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator/(const Time &next) {
    long temp = convertToNumber() / next.convertToNumber();
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator/(const long &next) {
    long temp = convertToNumber() / next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time Time::operator/(const int &next) {
    long temp = convertToNumber() / next;
    Time T;
    T.convertToTime(temp);
    return T;
}

Time& Time::operator+=(const Time &value) {
    *this = *this + value;
    return *this;
}

Time& Time::operator+=(const long &value) {
    *this = *this + value;
    return *this;
}

Time& Time::operator+=(const int &value) {
    *this = *this + value;
    return *this;
}

Time& Time::operator-=(const Time &value) {
    *this = *this - value;
    return *this;
}

Time& Time::operator-=(const long &value) {
    *this = *this - value;
    return *this;
}

Time& Time::operator-=(const int &value) {
    *this = *this - value;
    return *this;
}

bool Time::operator>(const Time& m) {
    return convertToNumber() > m.convertToNumber();
}

bool Time::operator<(const Time& m) {
    return convertToNumber() < m.convertToNumber();
}

bool Time::operator>=(const Time& m) {
    return convertToNumber() >= m.convertToNumber();
}

bool Time::operator<=(const Time& m) {
    return convertToNumber() <= m.convertToNumber();
}

std::ostream& operator<<(std::ostream& os, Time& dl) {
    return os << dl.print();
}

std::istream& operator>>(std::istream& is, Time& dl) {
    long value;
    is >> value;
    dl.convertToTime(value);
    return is;
}

//----------------------------------------------------------------------------------

VirtualTime::VirtualTime() {
    if (!readTime())
        last = 0;  //1000228870;
}

VirtualTime::~VirtualTime() {
    writeTime();
}

unsigned long VirtualTime::getCurrentTime() {
    return last;
}

unsigned long VirtualTime::addSecondsToCurrentTime(unsigned long seconds) {
    last += seconds;
    return last;
}

unsigned long VirtualTime::addHoursToCurrentTime(double hours) {
    unsigned long seconds = hours * 3600;
    last += seconds;
    return last;
}

void VirtualTime::writeTime() {
    std::ofstream out;
    out.open("Source/time.db");
    out << last << std::endl;
    out.close();
}

bool VirtualTime::readTime() {
    std::ifstream in;
    in.open("Source/time.db");
    if (in.fail())
        return false;
    in >> last;
    in.close();
    return true;
}

//----------------------------------------------------------------------------------

Money::Money(unsigned p) {
    wholeValue = 0;
    fracValue = 0;
    if (p>8)
        p=8;
    precision = p;
    multiplier = pow(10, p);
}

std::string Money::addSeperator(std::string input) {
    bool erased = false;
    if (input[0]=='-') {
        input.erase(0, 1);
        erased = true;
    }
    unsigned long size = input.size();
    while (size>3) {
        input.insert(size-3, ",");
        size -= 3;
    }
    if (erased)
        return '-' + input;
    return input;
}

Money Money::convertFraction(double value) {
    Money newItem;
    newItem.wholeValue = long(value);
    double f;
    f = (value - newItem.wholeValue) * multiplier;
    newItem.fracValue = long(std::round(f));
    return newItem;
}

Money Money::convertWhole(long value) {
    Money newItem;
    newItem.wholeValue = value;
    newItem.fracValue = 0;
    return newItem;
}

double Money::convert() const {
    double f = double(fracValue)/multiplier;
    double w = wholeValue;
    return w+f;
}

std::string Money::print() {
    std::string W = addSeperator(std::to_string(wholeValue));
    double f = roundDecimal(abs(double(fracValue)/double(multiplier)), 2);
    std::string F = std::to_string(f);
    if (wholeValue==0 && fracValue<0)
        W = "-" + W;
    return W + F.substr(1,3) + " " + "$";
}


double Money::roundDecimal(double var, int p)
{
    char str[80];
    if (p==0)
        sprintf(str, "%.0lf", var);
    else if (p==1)
        sprintf(str, "%.1lf", var);
    else if (p==2)
        sprintf(str, "%.2lf", var);
    else if (p==3)
        sprintf(str, "%.3lf", var);
    else
        sprintf(str, "%.4lf", var);
    sscanf(str, "%lf", &var);
    return var;
}

Money Money::operator=(const double &value) {
    *this = convertFraction(value);
    return *this;
}

Money Money::operator=(const long &value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator=(const int &value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator+(const Money &next) {
    double value = next.convert();
    double temp = convert();
    Money newItem = convertFraction(value+temp);
    return newItem;
}

Money Money::operator+(const long &value) {
    double temp = convert();
    Money newItem = convertFraction(temp+value);
    return newItem;
}

Money Money::operator+(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp+value);
    return newItem;
}

Money Money::operator+(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(value+temp);
    return newItem;
}

Money Money::operator-(const Money &next) {
    double value = next.convert();
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const long &value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator*(const Money &next) {
    double value = next.convert();
    double temp = convert();
    Money newItem = convertFraction(temp*value);
    return newItem;
}

Money Money::operator*(const long &value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator/(const Money &next) {
    double value = next.convert();
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const long &value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

bool Money::operator>(const Money &m) {
    return convert() > m.convert();
}

bool Money::operator<(const Money& m) {
    return convert() < m.convert();
}

bool Money::operator>=(const Money& m) {
    return convert() >= m.convert();
}

bool Money::operator<=(const Money& m) {
    return convert() <= m.convert();
}

Money& Money::operator+=(const Money &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const double &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const long &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const int &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator-=(const Money &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const double &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const long &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const int &value) {
    *this = *this - value;
    return *this;
}

std::ostream& operator<<(std::ostream& os, Money& dl) {
    return os << dl.print();
}

std::istream& operator>>(std::istream& is, Money& dl) {
    double value;
    is >> value;
    dl = dl.convertFraction(value);
    return is;
}

//----------------------------------------------------------------------------------

void printStats() {
    MiningParameters* MP = &MiningParameters::instance();
    VariableParameters* VP = &VariableParameters::instance();
    Money unit;
    unit = VP->getUnitPrice();
    int unitPerBlock = VP->getUnitPerNewBlock();
    long currentTotalHashPower = VP->currentTotalHashPower;
    long currentMinersPopulation = VP->currentMinersPopulation;
    long currentInactiveMinersPopulation = VP->currentInactiveMinersPopulation;
    long currentPoolsPopulation = VP->currentPoolsPopulation;
    int numberOfSoloMiners = VP->numberOfSoloMiners;
    int numberOfPoolMiners = VP->numberOfPoolMiners;
    int currentHighestMinerReputation = VP->currentHighestMinerReputation;
    int currentLowestMinerReputation = VP->currentLowestMinerReputation;
    //int numberOfAllViolations = VP->numberOfAllViolations;
    //int numberOfDetectedViolations = VP->numberOfDetectedViolations;
    int totalMinedBlocks = VP->totalMinedBlocks;
    Money totalRevenue;
    totalRevenue = VP->totalRevenue;
    Money totalCost;
    totalCost = VP->totalCost;
    std::cout << "\n================ Statistics ===============\n";
    std::cout << "Current Unit Value:       " << unit << std::endl;
    std::cout << "Unit Per New Block:       " << unitPerBlock << std::endl;
    std::cout << "Total Hash Power:         " << currentTotalHashPower << " TH/s" << std::endl;
    std::cout << "Active Miners:            " << currentMinersPopulation << std::endl;
    std::cout << "Inactive Miners:          " << currentInactiveMinersPopulation << std::endl;
    std::cout << "Pools:                    " << currentPoolsPopulation << std::endl;
    std::cout << "Solo vs. Pool Miners:     " << double(numberOfSoloMiners)/double(currentMinersPopulation)*100 << "% / " << double(numberOfPoolMiners)/double(currentMinersPopulation)*100 << "%" << std::endl;
    //std::cout << "Top Miner Hash Power:     " << currentHighestHashPower << std::endl;
    std::cout << "Highest Miner Reputation: " << currentHighestMinerReputation << std::endl;
    std::cout << "Lowest Miner Reputation:  " << currentLowestMinerReputation << std::endl;
    std::cout << "Total Mined Blocks:       " << totalMinedBlocks << std::endl;
    std::cout << "Total Value of Blocks:    " << totalRevenue << std::endl;
    std::cout << "Total Power Costs:        " << totalCost << std::endl;
    std::cout << "===========================================\n";
}
