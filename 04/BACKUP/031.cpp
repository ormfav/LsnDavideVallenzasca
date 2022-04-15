#include <functional>
#include <iostream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

#include "../config/031-conf.inl"

double f(Random& rnd){
    double z=rnd.Gauss(0,T);
    double s=S0*exp((INTREST_RATE-0.5*SIGMA*SIGMA)*T+SIGMA*sqrt(T)*z);
    return s > STRIKE_PRICE ? exp(-INTREST_RATE*T)*(s-STRIKE_PRICE) : 0;
}

double g(Random& rnd){
    double z=rnd.Gauss(0,T);
    double s=S0*exp((INTREST_RATE-0.5*SIGMA*SIGMA)*T+SIGMA*sqrt(T)*z);
    return s < STRIKE_PRICE ? exp(-INTREST_RATE*T)*(STRIKE_PRICE - s) : 0;
}

double h(Random& rnd){
    double t = (double)T/N_INTERVALS;
    double s=S0;

    for (int i=0; i<N_INTERVALS; ++i){ 
        double z=rnd.Gauss(0,1);
        s*=exp((INTREST_RATE-0.5*SIGMA*SIGMA)*t+SIGMA*sqrt(t)*z);
    }

    return s > STRIKE_PRICE ? exp(-INTREST_RATE*T)*(s-STRIKE_PRICE) : 0;
}

double i(Random& rnd){
    double t = (double)T/N_INTERVALS;
    double s=S0;

    for (int i=0; i<N_INTERVALS; ++i){
        double z=rnd.Gauss(0,1);
        s*=exp((INTREST_RATE-0.5*SIGMA*SIGMA)*t+SIGMA*sqrt(t)*z);
    }

    return s < STRIKE_PRICE ? exp(-INTREST_RATE*T)*(STRIKE_PRICE - s) : 0;
}

int main (int argc, char *argv[])
{
    Random rnd("lib/Random/Primes","lib/Random/seed.in");       
    
    dataBlocks callopt_direct(N_BLOCKS,STEPS_PER_BLOCK, bind(f,rnd));
    callopt_direct.ProgressiveAverage("03/Data/callopt_direct.csv");
    dataBlocks putopt_direct(N_BLOCKS,STEPS_PER_BLOCK, bind(g,rnd));
    putopt_direct.ProgressiveAverage("03/Data/putopt_direct.csv");
    
    dataBlocks callopt_discrete(N_BLOCKS,STEPS_PER_BLOCK, bind(h,rnd));
    callopt_discrete.ProgressiveAverage("03/Data/callopt_discrete.csv");
    dataBlocks putopt_discrete(N_BLOCKS,STEPS_PER_BLOCK, bind(i,rnd));
    putopt_discrete.ProgressiveAverage("03/Data/putopt_discrete.csv");
    
    return 0;
}   
