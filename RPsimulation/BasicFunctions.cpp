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

char* asctime_ct(const struct tm *timeptr) {
  static const char mon_name[][4] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[26];
  sprintf(result, "%.3s%3d, %d - %.2d:%.2d\n", mon_name[timeptr->tm_mon], timeptr->tm_mday, 1900 + timeptr->tm_year, timeptr->tm_hour, timeptr->tm_min);
  return result;
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
