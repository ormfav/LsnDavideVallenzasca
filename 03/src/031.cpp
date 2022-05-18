#include <algorithm>
#include <functional>
#include <iostream>
#include <cmath>
#include <ranges>
#include "../../lib/Random/random.h"
#include "../../lib/Point/point.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Misc/misc.h"

using namespace std;

#include "../in/031-options-conf.inl"
#include "../in/031-datablocking-conf.inl"


int main (int argc, char *argv[])
{
    /* Setting random number generator */
    Random rnd("03/in/Primes","03/in/seed.in");       

    /* Setting data blocking */
    //Eval option prices
    array<function<double(point<double,1>)>,2> f= {
        [](point<double,1> p){return p[0] > STRIKE_PRICE ? exp(-INTREST_RATE*T)*(p[0]-STRIKE_PRICE) : 0;},
        [](point<double,1> p){return p[0] < STRIKE_PRICE ? exp(-INTREST_RATE*T)*(STRIKE_PRICE-p[0]) : 0;}
    };
    //Direct move 
    auto m_drct = [&rnd](point<double,1>& p){p[0]=S0*exp((INTREST_RATE-0.5*SIGMA*SIGMA)*T+SIGMA*sqrt(T)*rnd.Gauss(0,T)); return 1;};
    //Discrete move
    auto m_disc = [&rnd](point<double,1>& p){ 
        double t = (double)T/N_INTERVALS;
        p[0]=S0;
        for (int i=0; i<N_INTERVALS; ++i){
            double z=rnd.Gauss(0,1);
            p[0]*=exp((INTREST_RATE-0.5*SIGMA*SIGMA)*t+SIGMA*sqrt(t)*z);
        }
        return 1;
    };
    dataBlocks<1,2> opt_drct(STEPS_PER_BLOCK, m_drct, f); 
    dataBlocks<1,2> opt_disc(STEPS_PER_BLOCK, m_disc, f); 
    
    /* Setting output files */
    array<ofstream,2> fout_drct;
    array<ofstream,2> fout_disc;
    array<string,2> path_drct = {"03/out/031-callopt_direct.csv","03/out/031-putopt_direct.csv"};
    array<string,2> path_disc= {"03/out/031-callopt_discrete.csv","03/out/031-putopt_discrete.csv"};
    for(int i=0;i<2;++i){
        fout_drct[i].open(path_drct[i]);
        FileCheck(fout_drct[i], path_drct[i]);
        fout_disc[i].open(path_disc[i]);
        FileCheck(fout_disc[i], path_disc[i]);
    }

    /* Data blocking */
    cout.setf(ios::unitbuf);
    cout<<"Block progression:\n";
    for(int i=0;i<N_BLOCKS;++i){
        opt_drct.Measure();
        opt_drct.EvalBlock(fout_drct);
        opt_disc.Measure();
        opt_disc.EvalBlock(fout_disc);
        cout<<i+1<<"\\"<<N_BLOCKS<<" ";
        if((i+1)%10==0)cout<<endl;
    }

    for(int i=0;i<2;++i){
        fout_drct[i].close();
        fout_disc[i].close();
    }
    
    
    return 0;
}   
