#include <cstdio>
#include <functional>
#include <iostream>
#include <cmath>
#include <fstream>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"
#include "../../lib/Misc/misc.h"
#include "../../lib/Metropolis/metropolis.h"


using namespace std;

/* #include "../config/051-conf.inl" */
#define DELTA_100 1
#define DELTA_210 2.5
#define SIGMA_100 0.7
#define SIGMA_210 1.5

#define N_BLOCKS 100
#define STEPS_PER_BLOCK 10000


int main (int argc, char *argv[])
{
    /* Initializing random number generator */
    Random rnd("lib/Random/Primes","lib/Random/seed.in");       

    /* Initializing data blocking */
    auto psi2_100 = [](point<double,3>p){return exp(-2.*p.Lenght());};
    auto psi2_210 = [](point<double,3>p){return p[2]*p[2]*exp(-p.Lenght());}; 
    array<function<double(point<double,3>)>,1> f = {[](point<double,3>p){return p.Lenght();}};

    array<double,3> p0_100_unif = {0,0,0};
    array<double,3> p0_210_unif = {0,0,1};
    array<double,3> p0_100_gauss = {0,0,0};
    array<double,3> p0_210_gauss = {0,0,1};

    array<dataBlocks<3,1>,4> r = {
        dataBlocks<3,1> (STEPS_PER_BLOCK,p0_100_unif,bind(Mrt2Unif<3>,placeholders::_1,psi2_100,rnd,DELTA_100),f),
        dataBlocks<3,1> (STEPS_PER_BLOCK,p0_210_unif,bind(Mrt2Unif<3>,placeholders::_1,psi2_210,rnd,DELTA_210),f),
        dataBlocks<3,1> (STEPS_PER_BLOCK,p0_100_gauss,bind(Mrt2Gauss<3>,placeholders::_1,psi2_100,rnd,SIGMA_100),f),
        dataBlocks<3,1> (STEPS_PER_BLOCK,p0_210_gauss,bind(Mrt2Gauss<3>,placeholders::_1,psi2_210,rnd,SIGMA_210),f)
    };

    /* Initializing output files */
    array<array<ofstream,1>,4> fout_r;
    array<string,4> path_r = {
        "05/out/051-r_100_unif.csv",
        "05/out/051-r_210_unif.csv",
        "05/out/051-r_100_gaus.csv",
        "05/out/051-r_210_gaus.csv"
    };

    array<ofstream,4> fout_points;
    array<string,4> path_points = {
        "05/out/051-points_100_unif.csv",
        "05/out/051-points_210_unif.csv",
        "05/out/051-points_100_gaus.csv",
        "05/out/051-points_210_gaus.csv"
    };

    for(int i=0;i<4;++i){
        fout_r[i][0].open(path_r[i]);
        FileCheck(fout_r[i][0],path_r[i]);
        fout_points[i].open(path_points[i]);
        FileCheck(fout_points[i],path_points[i]);
    }

    /* Data blocking */
    for (int i=0; i<N_BLOCKS; ++i) {
        cout<<"--- Block "<<(i+1)<<" ---\n";
        for(int j=0;j<4;++j){
            cout<<path_r[j].substr(11,10)+" acceptance: "<<r[j].Measure(10,fout_points[j])<<endl;
            r[j].EvalBlock(fout_r[j]);
        }
        cout<<endl;
    }

    for(auto& file : fout_r) file[0].close();
    for(auto& file : fout_points) file.close();

    return 0;
}   
