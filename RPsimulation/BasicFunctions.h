#ifndef BasicFunctions_h
#define BasicFunctions_h

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <cmath>

bool file_exist(const char *fileName);
char* asctime_ct(const struct tm *timeptr);
std::string moneyFormat(double amount);
void databaseInit();
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
