#include <functional>
#include <iostream>
#include <cmath>
#include <fstream>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"
#include "../../lib/Misc/misc.h"


using namespace std;

/* #include "../config/081-conf.inl" */
#define DELTA 2

#define N_BLOCKS 20
#define STEPS_PER_BLOCK 1000000

double AveH(double x, double sigma2, double mu){
    double x2 = x*x;
    double x4 = x2*x2;
    double mu2 = mu*mu;
    double mu4 = mu2*mu2;
    double amgis2 = 1/sigma2;
    double amgis4 = amgis2*amgis2;

    /* double potential_part = x4+2.5*x2; */
    double potential_part = 0.5*x2;
    double kinetic_part = (0.0625*(x4+x2*mu2+mu4)*amgis4 - 0.75*(x2+mu2)) / ( amgis2*(1 + exp((x2+mu2)*amgis2)) );
    return kinetic_part+potential_part;
}

int main (int argc, char *argv[])
{
    double mu=0;
    double sigma=1;
    /* Initializing random number generator */
    Random rnd("lib/Random/Primes","lib/Random/seed.in");       

    /* Initializing data blocking */
    auto psi2_gs = [](point<double,1>p){return exp(-2*p.Lenght());};
    array<function<double(point<double,1>)>,1> f = {[&mu,&sigma](point<double,1>p){return AveH(p[0],sigma*sigma,mu);}};

    array<double,1> p0 = {0};

    dataBlocks<1,1> energy(STEPS_PER_BLOCK,p0,bind(Mrt2Unif<1>,placeholders::_1,psi2_gs,rnd,DELTA),f);

    /* Initializing output files */
    array<ofstream,1> fout;
    string path = "08/out/081-energy.csv";
    fout[0].open(path);
    FileCheck(fout[0],path);

    /* Data blocking */
    for (int i=0; i<N_BLOCKS; ++i) {
        cout<<"--- Block "<<(i+1)<<" ---\n";
        cout<<"Acceptance: "<<energy.Measure()<<endl;
        energy.EvalBlock(fout);
        cout<<endl;
    }

    fout[0].close();

    return 0;
}   
