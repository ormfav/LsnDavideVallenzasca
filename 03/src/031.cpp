#include <functional>
#include <iostream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

#include "../config/031-options-conf.inl"
#include "../config/031-datablocking-conf.inl"

double CalloptDirect(Random& rnd){
    double z=rnd.Gauss(0,T);
    double s=S0*exp((INTREST_RATE-0.5*SIGMA*SIGMA)*T+SIGMA*sqrt(T)*z);
    return s > STRIKE_PRICE ? exp(-INTREST_RATE*T)*(s-STRIKE_PRICE) : 0;
}

double PutoptDirect(Random& rnd){
    double z=rnd.Gauss(0,T);
    double s=S0*exp((INTREST_RATE-0.5*SIGMA*SIGMA)*T+SIGMA*sqrt(T)*z);
    return s < STRIKE_PRICE ? exp(-INTREST_RATE*T)*(STRIKE_PRICE - s) : 0;
}

double CalloptDiscr(Random& rnd){
    double t = (double)T/N_INTERVALS;
    double s=S0;
    
    for (int i=0; i<N_INTERVALS; ++i){ 
        double z=rnd.Gauss(0,1);
        s*=exp((INTREST_RATE-0.5*SIGMA*SIGMA)*t+SIGMA*sqrt(t)*z);
    }
    
    return s > STRIKE_PRICE ? exp(-INTREST_RATE*T)*(s-STRIKE_PRICE) : 0;
}

double PutoptDiscr(Random& rnd){
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
    
    dataBlocks callopt_direct(N_BLOCKS,STEPS_PER_BLOCK, bind(CalloptDirect,rnd),"03/Data/031-callopt_direct.csv");
    dataBlocks putopt_direct(N_BLOCKS,STEPS_PER_BLOCK, bind(PutoptDirect,rnd),"03/Data/031-putopt_direct.csv");
    dataBlocks callopt_discrete(N_BLOCKS,STEPS_PER_BLOCK, bind(CalloptDiscr,rnd),"03/Data/031-callopt_discrete.csv");
    dataBlocks putopt_discrete(N_BLOCKS,STEPS_PER_BLOCK, bind(PutoptDiscr,rnd),"03/Data/031-putopt_discrete.csv");
    
    return 0;
}   
