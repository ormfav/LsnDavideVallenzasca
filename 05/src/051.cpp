#include <functional>
#include <iostream>
#include <cmath>
#include <fstream>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"
#include "../../lib/Misc/misc.h"


using namespace std;

/* #include "../config/051-conf.inl" */
#define DELTA_100 1
#define DELTA_210 2.5
#define SIGMA_100 0.7
#define SIGMA_210 1.5

#define N_BLOCKS 20
#define STEPS_PER_BLOCK 1000000


int main (int argc, char *argv[])
{
    /* Initializing random number generator */
    Random rnd("lib/Random/Primes","lib/Random/seed.in");       

    /* Initializing data blocking */
    auto psi2_100 = [](point<double,3>p){return exp(-2*p.Lenght());};
    auto psi2_210 = [](point<double,3>p){return p[2]*p[2]*exp(-p.Lenght());}; 
    array<function<double(point<double,3>)>,1> f = {[](point<double,3>p){return p.Lenght2();}};
    auto map_ave = [](double ave){return sqrt(ave);};
    auto map_err = [](double ave, double err){return 1.;}; //fare propagazione
    array<double,3> p0_100_unif = {1,1,1};
    array<double,3> p0_210_unif = {1,1,1};
    array<double,3> p0_100_gauss = {1,1,1};
    array<double,3> p0_210_gauss = {1,1,1};

    dataBlocks<3,1> r2_100_unif(STEPS_PER_BLOCK,p0_100_unif,bind(Mrt2Unif<3>,placeholders::_1,psi2_100,rnd,DELTA_100),f);
    dataBlocks<3,1> r2_210_unif(STEPS_PER_BLOCK,p0_210_unif,bind(Mrt2Unif<3>,placeholders::_1,psi2_210,rnd,DELTA_210),f);
    dataBlocks<3,1> r2_100_gauss(STEPS_PER_BLOCK,p0_100_gauss,bind(Mrt2Gauss<3>,placeholders::_1,psi2_100,rnd,SIGMA_100),f);
    dataBlocks<3,1> r2_210_gauss(STEPS_PER_BLOCK,p0_210_gauss,bind(Mrt2Gauss<3>,placeholders::_1,psi2_210,rnd,SIGMA_210),f);
    array<dataBlocks<3,1>,4> r = {r2_100_unif, r2_210_unif, r2_100_gauss, r2_210_gauss};

    /* Initializing output files */
    array<array<ofstream,1>,4> fout;
    array<string,4> path = {"05/out/051-r2_100_unif.csv",
                            "05/out/051-r2_210_unif.csv",
                            "05/out/051-r2_100_gaus.csv",
                            "05/out/051-r2_210_gaus.csv"};
    for(int i=0;i<4;++i){
        fout[i][0].open(path[i]);
        FileCheck(fout[i][0],path[i]);
    }

    /* Data blocking */
    for (int i=0; i<N_BLOCKS; ++i) {
        cout<<"--- Block "<<(i+1)<<" ---\n";
        for(int j=0;j<4;++j){
            cout<<path[j].substr(11,11)+" acceptance: "<<r[j].Measure()<<endl;
            r[j].Measure();
            r[j].Map(0,map_ave,map_err);
            r[j].EvalBlock(fout[j]);
        }
    }

    for(auto& file : fout) file[0].close();

    return 0;
}   
