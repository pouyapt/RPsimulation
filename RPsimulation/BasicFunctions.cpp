#include "BasicFunctions.h"

bool is_file_exist(const char *fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

void databaseInit() {
    if (!is_file_exist("Data")) {
        system("mkdir Data");
    }
}

char* convertToDataTime(const struct tm *timeptr) {
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
    int n = 0;
    double p = 0;
    while (p<targetProbability)
        p = atLeastOneOccurencePerNTrial(probability, ++n);
    return n;
}

double calculatePopulationGrowth(unsigned p, unsigned m, double k) {
    return k*(1-(double(p)/double(m)));
}

double sigmoidFunction(long x, long carryingCapacity, double range, double squeezeFactor) {
    double e = 2.7182818285;
    double y = (range / (1+pow(e, (1/squeezeFactor)*(x-carryingCapacity)))) - (range/2);
    return y;
}
