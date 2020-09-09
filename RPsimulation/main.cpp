#include "interactions.h"



void minerAndPoolsTest(MinerPopulation* MP, Pools* P) {
    int n = 1951;
    while (n) {
        for (int i=0; i<P->size(); i++) {
            unsigned k = rand();
            (*P)[i]->pickMiner((*MP)[k]);
        }
        n--;
    }
}

void minerAndPoolsTest(core::list<PoolManager*> &P, core::list<Miner*> M) {
    core::Random* gen = &core::Random::instance();
    int n = 1500;
    while (n) {
        for (auto i=0; i<P.size(); i++) {
            long k = gen->select_random_index(0, M.size()-1);
            P[i]->pickMiner(M[k]);
        }
        n--;
    }
}


int main() {

    core::list<PoolManager*> P;
    core::list<Miner*> M;


    for (int i=0; i<25000; i++) {
        Miner* temp = new Miner;
        M.push_back(temp);
    }
    for (int i=0; i<32; i++) {
        PoolManager* temp = new PoolManager;
        P.push_back(temp);
    }

    minerAndPoolsTest(P, M);

    for (int i=0; i<M.size(); i++) {
        if (M[i]->pool!=NULL)
            M[i]->pool->releaseMiner(M[i]);
    }


    for (int i=0; i<M.size(); i++) {
        M[i]->print();
    }

    for (int i=0; i<P.size(); i++) {
        P[i]->print();
    }

    for (int i=0; i<M.size(); i++) {
        delete M[i];
    }
    for (int i=0; i<P.size(); i++) {
        delete P[i];
    }
    
//    core::list<int*> I;
//
//    for (int i=0; i<6; i++) {
//        int* k = new int;
//        *k = i;
//        I.push_back(k);
//    }
//
//    std::cout << " " << *I.pop(2) << std::endl;//2
//    std::cout << " " << *I.pop(4) << std::endl;//5
//    std::cout << " " << *I.pop(3) << std::endl;//4
//    std::cout << " " << *I.pop(2) << std::endl;//3
//    std::cout << " " << *I.pop(1) << std::endl;//1
//
//    for (int i=0; i<I.size(); i++) {
//        std::cout << *I[i] << std::endl;
//    }
}

