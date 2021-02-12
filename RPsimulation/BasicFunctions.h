#ifndef BasicFunctions_h
#define BasicFunctions_h

#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <cmath>

bool file_exist(std::string fileName);
char* asctime_ct(const struct tm *timeptr);
std::string moneyFormat(double amount);
void folder();
double round_(double var);
double atLeastOneOccurencePerNTrial(double px, int n);
int noOfRequiredTrials(double px, double targetP);
double calculatePopulationGrowth(unsigned p, unsigned m, double k);
double sigmoid(double variable, double MaxValue, double steepness, double midPoint, double offset);
double sigmoidDeravative(double variable, double MaxValue, double steepness, double midPoint);
double confidenceIntervalLowerBound(double x, double n, int confidence);
double confidenceIntervalUpperBound(double x, double n, int confidence);

class Money;
class Time;
class Random;
class Miner;
class PoolManager;
class MinerPopulation;
class Reputation;
class Pools;
class Game;
class MasterTime;
class BW_Attack;

#endif /* foundation_hpp */
