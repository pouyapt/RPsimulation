#include "miner.h"

int main()
{
    system("mkdir temp");
    auto DB = MinerPopulation(20, "temp/rr.txt");
    std::cout << DB.totalHashPower() << std::endl;
}




