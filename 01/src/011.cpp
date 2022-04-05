#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

int main (int argc, char *argv[])
{
    #include "../config/011-ave_var-conf.inl"
    Random rnd("lib/Random/Primes","lib/Random/seed.in");
 
    //calcolo media-errore
    //posso passare direttamente Rannyu?
    auto f_ave=[&rnd](){return rnd.Rannyu();};
    dataBlocks average(N_BLOCKS,STEPS_PER_BLOCK,f_ave);
    average.ProgressiveAverage("01/Data/011-progressive_averages.csv");

    //calcolo varianza-errore
    auto f_var=[&rnd, &average](){
        double x=rnd.Rannyu();
        double ave=average.Average();
        return (x-ave)*(x-ave);
    };
    dataBlocks variance(N_BLOCKS,STEPS_PER_BLOCK,f_var);
    variance.ProgressiveAverage("01/Data/011-progressive_variances.csv");

    //test chi2
    #include "../config/011-chi2-conf.inl"
    if(N_INTERVALS>=THROWS){
        cerr<<"Se il numero di lanci è inferiore al numero di intervalli il test non è statisticamente significativo\n";
        exit(1);
    }
//reset rangen per riproducibilità?
    ofstream fout;
    fout.open("01/Data/011-chi2.dat");
    for (int i=0; i<N_TESTS; ++i) {
        int n[N_INTERVALS]={0};
        double chi2=0;
        for (int j=0; j<THROWS; ++j) {
            double x=rnd.Rannyu();
            n[(int)(x*N_INTERVALS)]++;
        }
        for (int j=0; j<N_INTERVALS; ++j) 
            chi2+=(n[j]-THROWS/N_INTERVALS)*(n[j]-THROWS/N_INTERVALS);
        chi2*=(double)N_INTERVALS/THROWS; 
        fout<<chi2<<endl;
    }
    fout.close();

    return 0;
}
