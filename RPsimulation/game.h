#ifndef game_h
#define game_h

#include "structures.h"

#define HPperPC 0.45    // Hash Power per Power Consumption
#define MT 0.126        // Average mining time for each block
#define UP 9237         // Crypto Unit Price

Miner* mine(MinerPopulation & DB);
bool violationDetect(Miner* target);
void mineGame(MinerPopulation & DB);

#endif
