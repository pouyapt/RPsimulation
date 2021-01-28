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
    readTime();
}

VirtualTime::~VirtualTime() {
    writeTime();
}

long VirtualTime::getCurrentTime() {
    return last;
}

long VirtualTime::addSecondsToCurrentTime(long seconds) {
    last += seconds;
    return last;
}

long VirtualTime::addHoursToCurrentTime(double hours) {
    long seconds = hours * 3600;
    last += seconds;
    return last;
}

void VirtualTime::writeTime() {
    std::ofstream out;
    out.open("Data/time.db");
    out << last << std::endl;
    out.close();
}

bool VirtualTime::readTime() {
    std::ifstream in;
    in.open("Data/time.db");
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

Money Money::operator=(const double &&value) {
    *this = convertFraction(value);
    return *this;
}

Money Money::operator=(const long &value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator=(const long &&value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator=(const int &value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator=(const int &&value) {
    *this = convertWhole(value);
    return *this;
}

Money Money::operator+(const Money &next) {
    double value = next.convert();
    double temp = convert();
    Money newItem = convertFraction(value+temp);
    return newItem;
}

Money Money::operator+(const Money &&next) {
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

Money Money::operator+(const long &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp+value);
    return newItem;
}

Money Money::operator+(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp+value);
    return newItem;
}

Money Money::operator+(const int &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp+value);
    return newItem;
}

Money Money::operator+(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(value+temp);
    return newItem;
}

Money Money::operator+(const double &&value) {
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

Money Money::operator-(const Money &&next) {
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

Money Money::operator-(const long &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const int &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(temp-value);
    return newItem;
}

Money Money::operator-(const double &&value) {
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

Money Money::operator*(const Money &&next) {
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

Money Money::operator*(const long &&value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const int &&value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(value*temp);
    return newItem;
}

Money Money::operator*(const double &&value) {
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

Money Money::operator/(const Money &&next) {
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

Money Money::operator/(const long &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const int &value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const int &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const double &value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

Money Money::operator/(const double &&value) {
    double temp = convert();
    Money newItem = convertFraction(temp/value);
    return newItem;
}

bool Money::operator>(const Money &m) {
    return convert() > m.convert();
}

bool Money::operator>(const Money &&m) {
    return convert() > m.convert();
}

bool Money::operator<(const Money &m) {
    return convert() < m.convert();
}

bool Money::operator<(const Money &&m) {
    return convert() < m.convert();
}

bool Money::operator>=(const Money &m) {
    return convert() >= m.convert();
}

bool Money::operator>=(const Money &&m) {
    return convert() >= m.convert();
}

bool Money::operator<=(const Money &m) {
    return convert() <= m.convert();
}

bool Money::operator<=(const Money &&m) {
    return convert() <= m.convert();
}

Money& Money::operator+=(const Money &value) {
    *this = *this + value;
    return *this;
}

Money& Money::operator+=(const Money &&value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const double &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const double &&value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const long &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const long &&value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const int &value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator+=(const int &&value) {
    *this = *this + value;
    return *this;
}

Money& Money:: operator-=(const Money &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const Money &&value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const double &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const double &&value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const long &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const long &&value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const int &value) {
    *this = *this - value;
    return *this;
}

Money& Money:: operator-=(const int &&value) {
    *this = *this - value;
    return *this;
}

std::ostream& operator<<(std::ostream& os, Money& dl) {
    return os << dl.print();
}

std::ostream& operator<<(std::ostream& os, Money&& dl) {
    return os << dl.print();
}

std::istream& operator>>(std::istream& is, Money& dl) {
    double value;
    is >> value;
    dl = dl.convertFraction(value);
    return is;
}

std::istream& operator>>(std::istream& is, Money&& dl) {
    double value;
    is >> value;
    dl = dl.convertFraction(value);
    return is;
}

//-------------------------------------------------------------------------------------

double Trust::mu_x_B(double x) {
    return (p->theta-p->eta)/(p->beta+1)*(x+1)+p->eta;
}

double Trust::mu_x_N(double x)  {
    return p->theta;
}

double Trust::mu_x_G(double x)  {
    return (p->kappa-p->theta)/(1-p->epsilon-p->alpha)*(x-p->alpha)+p->theta;
}

double Trust::mu_x_1(double x)  {
    return (p->kappa/p->epsilon)*(1-x-p->epsilon)+p->kappa;
}

double Trust::mu_prime_x_1(double x)  {
    return (p->kappa/p->epsilon)*(x+1);
}

double Trust::mu_prime_x_B(double x)  {
    return (p->theta-p->kappa)/(p->beta-p->epsilon+1)*(x-p->epsilon+1)+p->kappa;
}

double Trust::mu_prime_x_N(double x)  {
    return p->theta;
}

double Trust::mu_prime_x_G(double x)  {
    return (p->eta-p->theta)/(1-p->alpha)*(x-p->alpha)+p->theta;
}

double Trust::cooperation(double x) {
    if (x < p->beta)
        return x+mu_x_B(x);
    if (x >= p->beta && x <= p->alpha)
        return x+mu_x_N(x);
    if (x > p->alpha && x <= 1-p->epsilon)
        return x+mu_x_G(x);
    return x+mu_x_1(x);
}

double Trust::defection(double x) {
    if (x > p->alpha)
        return x-mu_prime_x_G(x);
    if (x >= p->beta && x <= p->alpha)
        return x-mu_x_N(x);;
    if (x < p->beta && x >= p->epsilon-1)
        return x-mu_prime_x_B(x);
    return x-mu_prime_x_1(x);
}

//----------------------------------------------------------------------------------

Stats::~Stats() {
    addNewStatsToCsvFile();
}

long Stats::getCurrentTotalHashPower() {
    return current.totalHashPower;
}

long Stats::getCurentMinersPopulation() {
    return current.minersPopulation;
}

long Stats::getCurrentPoolsPopulation() {
    return current.poolsPopulation;
}

Money Stats::getUnitPrice() {
    return current.unitPrice;
}

double Stats::getUnitPerNewBlock() {
    return current.unitPerNewBlock;
}
void Stats::updateNumberOfPoolMiners(int i) {
    current.numberOfPoolMiners += i;
}

bool Stats::inReputationMode() {
    return reputationMode;
}

void Stats::printCurrentStats() {
    std::cout << "\n=================== Statistics ==================\n";
    std::cout << "Current Unit Value:                   " << current.unitPrice << std::endl;
    std::cout << "Unit Per New Block:                   " << getUnitPerNewBlock() << std::endl;
    std::cout << "Total Network Hash Power:             " << current.totalHashPower << " TH/s" << std::endl;
    std::cout << "Present Miners:                       " << current.minersPopulation << std::endl;
    std::cout << "Past Miners:                          " << current.inactiveMinersPopulation << std::endl;
    std::cout << "Number of Pools:                      " << current.poolsPopulation << std::endl;
    std::cout << "Solo / Pool Miners:                   " << double(current.minersPopulation - current.numberOfPoolMiners)/double(current.minersPopulation)*100 << "% / " << double(current.numberOfPoolMiners)/double(current.minersPopulation)*100 << "%" << std::endl;
    std::cout << "Last Generated Block:                 " << convertToDate_Time(current.lastGeneratedBlockTime);
    std::cout << "Highest Miner Reputation:             " << current.highestMinerReputation << std::endl;
    std::cout << "Lowest Miner Reputation:              " << current.lowestMinerReputation << std::endl;
    std::cout << "Total Mined Blocks:                   " << current.totalMinedBlocks << std::endl;
    std::cout << "Total Mining Power Costs:             " << current.totalCost << std::endl;
    std::cout << "Current Value of All Blocks:          " << current.unitPrice * getUnitPerNewBlock() * current.totalMinedBlocks << std::endl;
    std::cout << "Dishonest Activities:                 " << current.dishonestActivityCount << std::endl;
    std::cout << "Detected Dishonest Activities:        " << current.detectedDishonestActivityCount << std::endl;
    std::cout << "False Detected Dishonest Activities:  " << current.falseDetectedDishonestActivityCount << std::endl;
    std::cout << "=================================================\n";
}

void Stats::addNewStatsToCsvFile() {
    statFileInit();
    std::fstream uidlFile(filename, std::fstream::in | std::fstream::out | std::fstream::app);
     if (uidlFile.is_open()) {
         for (auto i=0; i<snapShots.size(); i++) {
             uidlFile << snapShots[i].totalMinedBlocks << ","
                      << snapShots[i].time << ","
                      << snapShots[i].unitPrice.convert() << ","
                      << snapShots[i].unitPerNewBlock << ","
                      << snapShots[i].totalHashPower << ","
                      << snapShots[i].minersPopulation << ","
                      << snapShots[i].poolsPopulation << ","
                      << snapShots[i].numberOfPoolMiners << ","
                      << snapShots[i].unitPrice.convert()*snapShots[i].unitPerNewBlock*snapShots[i].totalMinedBlocks << ","
                      << snapShots[i].totalCost.convert() << ","
                      << snapShots[i].highestMinerReputation << ","
                      << snapShots[i].lowestMinerReputation << ","
                      << snapShots[i].dishonestActivityCount << ","
                      << snapShots[i].detectedDishonestActivityCount << ","
                      << snapShots[i].falseDetectedDishonestActivityCount << "\n";
         }
         uidlFile.close();
     }
     else {
       std::cout << "Cannot save to 'stat_snapshots.csv' file" << std::endl;
     }
}

void Stats::saveSnapShot() {
    snapShots.push_back(current);
}

void Stats::statFileInit() {
    if (!file_exist("Output/stat_snapshots.csv")) {
        std::ofstream out;
        out.open("Output/stat_snapshots.csv");
        out << "round,"
            << "unix_time,"
            << "unit_price,"
            << "units_per_new_block,"
            << "total_hash_power,"
            << "number_of_miners,"
            << "number_of_pools,"
            << "pool_miners,"
            << "total_blocks_value_in_dollar,"
            << "total_miner_power_cost,"
            << "highest_miner_reputation,"
            << "lowest_miner_reputation,"
            << "dishonest_activities,"
            << "detected_dishonest_activities,"
            << "false_detected_dishonest_activities\n";
    }
}
