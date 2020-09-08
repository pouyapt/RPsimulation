#ifndef BasicFunctions_h
#define BasicFunctions_h

#include "parameters.h"

bool file_exist(const char *fileName);
char* asctime_ct(const struct tm *timeptr);
char* convertToDate_Time(long time);
void databaseInit();
void statFileInit();
double round_(double var);
double atLeastOneOccurencePerNTrial(double px, int n);
int noOfRequiredTrials(double px, double targetP);
double calculatePopulationGrowth(unsigned p, unsigned m, double k);
double sigmoid(double variable, double MaxValue, double steepness, double midPoint, double offset);
double sigmoidDeravative(double variable, double MaxValue, double steepness, double midPoint);

class Money;
class Time;
class Random;
class Miner;
class PoolManager;
class MinerPopulation;
class Pools;
class Game;
class MasterTime;

#endif /* foundation_hpp */
