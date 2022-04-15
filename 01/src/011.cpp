#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

int main (int argc, char *argv[])
{
    #include "../in/011-ave_var-conf.inl"
    Random rnd("01/in/Primes","01/in/seed.in");
 
    //calcolo media-errore
    //posso passare direttamente Rannyu?
    dataBlocks average(N_BLOCKS,STEPS_PER_BLOCK,[&rnd](){return rnd.Rannyu();},"01/out/011-progressive_averages.csv");

    //calcolo varianza-errore
    auto f_var=[&rnd, &average](){
        double x=rnd.Rannyu();
        double ave=average.Stats(0).first;
        return (x-ave)*(x-ave);
    };
    dataBlocks variance(N_BLOCKS,STEPS_PER_BLOCK,f_var,"01/out/011-progressive_variances.csv");

    //test chi2
    #include "../in/011-chi2-conf.inl"
    if(N_INTERVALS>=THROWS){
        cerr<<"Se il numero di lanci è inferiore al numero di intervalli il test non è statisticamente significativo\n";
        exit(1);
    }
    ofstream fout;
    fout.open("01/out/011-chi2.dat");
    if(!fout.is_open()){
        cerr << "PROBLEM: Unable to open 01/out/011-chi2.dat" << endl;
        exit(1);
    }
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
