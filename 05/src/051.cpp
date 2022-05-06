#include <functional>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"

using namespace std;

/* #include "../config/051-conf.inl" */
#define N_STEPS 1000000

//T analitica (uguale a quella di rnd)
//p funzione analitica
/* point <double,3> Metropolis(Random& rnd, point<double, 3> x ){} */


int main (int argc, char *argv[])
{
    Random rnd("lib/Random/Primes","lib/Random/seed.in");       
    point<double, 3> x;
    x[0]=1;
    x[1]=1;
    x[2]=1;
    double delta=1;
    int acc=0;
    ofstream fout("05/Data/points.csv");
    fout<<"x,y,z\n";

    auto psi2_100 = [](point<double,3>p){return exp(-2*p.Lenght()); };
    auto psi2_210 = [](point<double,3>p){
        double r=p.Lenght();
        return r*r*exp(-r)/(p[1]*p[1]/(p[0]*p[0])+1); 
    };

    for (int i=0; i<N_STEPS; ++i) {
        point<double, 3> y;
        for(int j=0; j<3; ++j) 
            y[j]=rnd.Rannyu(x[j]-delta,x[j]+delta);

        if (rnd.Rannyu()<=min(1.,psi2_100(y)/psi2_100(x))){
            x=y;
            acc++;
        }
        cout<<x.Lenght()<<endl;
        fout<<x[0]<<","<<x[1]<<","<<x[2]<<endl;
    }
    cout<<"Acceptance: "<<(double)acc/N_STEPS*100<<"%\n";
     
    
    /* dataBlocks callopt_direct(N_BLOCKS,STEPS_PER_BLOCK, bind(f,rnd)); */
    /* callopt_direct.ProgressiveAverage("03/Data/callopt_direct.csv"); */
    
    return 0;
}   
