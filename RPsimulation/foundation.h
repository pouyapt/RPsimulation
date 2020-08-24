#ifndef foundation_hpp
#define foundation_hpp

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread>

bool is_file_exist(const char *fileName);
char* asctime_c(const struct tm *timeptr);
void databaseInit();
double round_(double var);
double atLeastOneOccurencePerNTrial(double px, int n);
int noOfRequiredTrials(double px, double targetP);

class Money;
class Time;
class Random;
class Miner;
class PoolManager;
class MinerPopulation;
class Pools;
class Game;

#endif /* foundation_hpp */
