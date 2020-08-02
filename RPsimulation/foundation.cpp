#include "foundation.h"


bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void databaseInit() {
    if (!is_file_exist("Data"))
        system("mkdir Data");
}

//----------------------------------------------------------------------------------

EntityParameters::EntityParameters() {
    miningPowerMean = 123;
    miningPowerStd = 7476;
    miningPowerMin = 3;
    miningPowerLowRange = 72;
    dishonestyFactorMean = 0;
    dishonestyFactorStd = 10;
    dishonestyFactorMin = 0;
    minersPopulationGrowthMean = 131;
    minersPopulationGrowthStd = 86;
    minersPopulationGrowthMin = 0;
    powerConsumptionMean = 0.05;
    powerConsumptionStd = 0.02;
    powerConsumptionMin = 0.02;
    powerConsumptionMax = 0.11;
    poolFeeMin = 14;
    poolFeeMax = 23;
}

int EntityParameters::getMiningPowerPars(std::string parameter) {
    if (parameter=="mean")
        return miningPowerMean;
    else if (parameter=="std")
        return miningPowerStd;
    else if (parameter=="min")
        return miningPowerMin;
    else
        return miningPowerLowRange;
}

int EntityParameters::getDishonestyFactorPars(std::string parameter) {
    if (parameter=="mean")
        return dishonestyFactorMean;
    else if (parameter=="std")
        return dishonestyFactorStd;
    else
        return dishonestyFactorMin;
}

int EntityParameters::getMinersPopulationGrowthPars(std::string parameter) {
    if (parameter=="mean")
        return minersPopulationGrowthMean;
    else if (parameter=="std")
        return minersPopulationGrowthStd;
    else
        return minersPopulationGrowthMin;
}

double EntityParameters::getPowerConsumptionPars(std::string parameter) {
    if (parameter=="mean")
        return powerConsumptionMean;
    else if (parameter=="std")
        return powerConsumptionStd;
    else if (parameter=="min")
        return powerConsumptionMin;
    else
        return powerConsumptionMax;
}

double EntityParameters::getPoolFeesPars(std::string parameter) {
    if (parameter=="min")
        return poolFeeMin;
    else
        return poolFeeMax;
}

//----------------------------------------------------------------------------------

MiningParameters::MiningParameters() {
    unitPrice = 9237;
    unitPerNewBlock = 50;
    aveMiningTime = 0.126;
}

double MiningParameters::getUnitPrice() {
    return unitPrice;
}

double MiningParameters::getUnitPerNewBlock() {
    return unitPerNewBlock;
}


double MiningParameters::getAveMiningTime() {
    return aveMiningTime;
}

//----------------------------------------------------------------------------------

PopulationParameters::PopulationParameters() {
    defaultMinersPopulation = 43256;
    defaultNumberOfPool = 76;
    
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

