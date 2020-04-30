#include "miner.h"
#include "generators.h"
#include "listDS.h"


int main()
{
    core::random myRan;
    core::list<Miner> myList;
    Miner m1;

    for (int i=0; i<20; i++) {
        m1.initialize();
        myList.push_back(m1);
    }

    double (*method)(Miner) = [](Miner a){return a.dFact();};
    myList.sort(method);
    for (int i=0; i<myList.size(); i++) {
        myList[i].print();
    }
}
