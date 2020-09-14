#include "BasicFunctions.h"

bool file_exist(const char *fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void databaseInit() {
    if (!file_exist("Data")) {
        system("mkdir Data");
    }
}

void statFileInit() {
    if (!file_exist("stats.csv")) {
        std::ofstream out;
        out.open("stats.csv");
        out << "unix_time" << "," << "unit_price" << "," << "units_per_block" << "," << "total_hash_power" << "," << "miners" << "," << "pools" << "," << "solo_miners" << "," << "revenue" << "," << "cost" << std::endl;
    }
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

double expectedValue(double mean, double std) {
    return 0;
}

double atLeastOneOccurencePerNTrial(double probability, int noOfTrials) {
    return 1 - pow(1-probability, noOfTrials);
}

int noOfRequiredTrials(double probability, double targetProbability) {
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
