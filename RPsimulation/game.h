#ifndef game_h
#define game_h

#include "structures.h"

#define HPperPC 0.35    //Hash Power per Power Consumption
#define MT 0.166

float unitPrice = 9237;

Miner* mine(MinerPopulation & DB);
bool violationDetect(Miner* target);
void mineGame(MinerPopulation & DB);
#endif
