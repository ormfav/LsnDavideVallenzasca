#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

int main (int argc, char *argv[])
{
    /*------------------------------*/
    /* Evaluatng Average - Variance */
    /*------------------------------*/
    #include "../in/011-ave_var-conf.inl"

    //Initializing random number generator
    Random rnd("01/in/Primes","01/in/seed.in");
 
    //Initializing data blocking
    auto m = [&rnd](point<double,1>& p){p[0]=rnd.Rannyu(); return (bool)1;};
    auto e0 = [](point<double,1> p){return p[0];};
    auto e1 = [](point<double,1> p){return (p[0]-0.5)*(p[0]-0.5);};
    array<function<double(point<double,1>)>,2> e = {e0,e1};
    dataBlocks<1,2> ave_var(STEPS_PER_BLOCK,m,e);

    //initializing output files
    array<ofstream,2> fout_ave_var;
    array<string,2> path = {"01/out/011-progressive_averages.csv","01/out/011-progressive_variances.csv"};
    for(int i=0;i<2;++i) {
        fout_ave_var[i].open(path[i]);
        if (!fout_ave_var[i].is_open()){
            cout<<"PROBLEM: unable to open "+path[i]<<endl;
            exit(1);
        }
    }

    //Data blocking
    for(int i=0; i<N_BLOCKS; ++i){
        ave_var.Measure();
        ave_var.EvalBlock(fout_ave_var);
    }

    fout_ave_var[0].close();
    fout_ave_var[1].close();

    /*-----------*/
    /* chi2 test */
    /*-----------*/
    #include "../in/011-chi2-conf.inl"
    if(N_INTERVALS>=THROWS){
        cerr<<"Se il numero di lanci è inferiore al numero di intervalli il test non è statisticamente significativo\n";
        exit(1);
    }

    ofstream fout_chi2;
    fout_chi2.open("01/out/011-chi2.dat");
    if(!fout_chi2.is_open()){
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
        fout_chi2<<chi2<<endl;
    }
    fout_chi2.close();

    return 0;
}
