#include "BasicFunctions.h"

bool file_exist(std::string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void folder() {
    namespace s = std::__fs::filesystem;
    const s::path& output = "./output";
    const s::path& data = "./data";
    if (!s::is_directory(output))
        create_directories(output);
    if (!s::is_directory(data))
        create_directories(data);
}

char* asctime_ct(const struct tm *timeptr) {
    static const char mon_name[][4] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static char result[26];
    sprintf(result, "%.3s%3d, %d - %.2d:%.2d\n", mon_name[timeptr->tm_mon], timeptr->tm_mday, 1900 + timeptr->tm_year, timeptr->tm_hour, timeptr->tm_min);
    return result;
}

std::string moneyFormat(double amount) {
    std::string m;
    bool negative = amount < 0;
    amount = abs(amount);
    int whole = int(amount);
    double fraction = amount - whole;
    int fraction_int = fraction*100;
    if (fraction_int<10)
        m += std::to_string(whole) + ".0" + std::to_string(fraction_int);
    else
        m += std::to_string(whole) + "." + std::to_string(fraction_int);
    int count = 0;
    for (auto i=m.size()-3; i>1; i--) {
        count++;
        if (count==3) {
            m.insert(i-1, ",");
            count = 0;
        }
    }
    if (negative && fraction_int)
        m.insert(0, "-");
    return m + " $";
}

double atLeastOneOccurencePerNTrial(double probability, int noOfTrials) {
    return 1 - pow(1-probability, noOfTrials);
}

int noOfRequiredTrials(double probability, double targetProbability) {
    if (probability<=0.0001)
        return INT_MAX;
    int n = 0;
    double p = 0;
    while (p<targetProbability)
        p = atLeastOneOccurencePerNTrial(probability, ++n);
    return n;
}

double calculatePopulationGrowth(unsigned p, unsigned m, double k) {
    return k*(1-(double(p)/double(m)));
}

double sigmoid(double variable, double MaxValue, double steepness, double midPoint, double offset) {
    return (MaxValue / (1+exp(-1*steepness*(variable-midPoint)))) - offset;
}

double sigmoidDeravative(double variable, double MaxValue, double steepness, double midPoint) {
    return (steepness*MaxValue*exp(steepness*(variable-midPoint)) / pow(1+exp(steepness*(variable-midPoint)), 2));
}

double confidenceIntervalLowerBound(double x, double n, int confidence) {
    double z;
    if (confidence==90)
        z = 1.645;
    else if (confidence==95)
        z = 1.96;
    else if (confidence==98)
        z = 2.326;
    else
        z = 2.576;
    double p_hat = x/n;
    return p_hat - z*sqrt((p_hat*(1-p_hat))/n);
}

double confidenceIntervalUpperBound(double x, double n, int confidence) {
    double z;
    if (confidence==90)
        z = 1.645;
    else if (confidence==95)
        z = 1.96;
    else if (confidence==98)
        z = 2.326;
    else
        z = 2.576;
    double p_hat = x/n;
    return p_hat + z*sqrt((p_hat*(1-p_hat))/n);
}
