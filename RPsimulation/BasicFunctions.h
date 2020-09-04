#ifndef BasicFunctions_h
#define BasicFunctions_h

#include "parameters.h"

bool is_file_exist(const char *fileName);
char* convertToDataTime(const struct tm *timeptr);
void databaseInit();
double round_(double var);
double atLeastOneOccurencePerNTrial(double px, int n);
int noOfRequiredTrials(double px, double targetP);
double calculatePopulationGrowth(unsigned p, unsigned m, double k);
double sigmoidFunction(long x, long carryingCapacity, double range, double squeezeFactor);

class Money;
class Time;
class Random;
class Miner;
class PoolManager;
class MinerPopulation;
class Pools;
class Game;

#endif /* foundation_hpp */
