#include <iostream>
#include <fstream>
#include <array>
#include "../../lib/Random/random.h"
#include "../../lib/Misc/misc.h"

using namespace std;

int main (int argc, char *argv[])
{
    #include "../in/012-conf.inl"
    int N[4] = {1,2,10,100};
    char lastchar[4]={',', ',', ',', '\n'}; //formatting output files

    Random rnd("lib/Random/Primes","lib/Random/seed.in");
 
    array<ofstream,3> fout;
    array<string,3> path={
        "01/out/012-unif.csv",
        "01/out/012-exp.csv",
        "01/out/012-cauchy.csv"
    };

    for(int i=0;i<3;i++){
        fout[i].open(path[i]);
        FileCheck(fout[i],path[i]);
        fout[i]<<"N=1,N=2,N=10,N=100\n";
    }

    for (int i=0; i<T; ++i) {
        for(int j=0; j<4; ++j){
            double sum_unif=0.; 
            double sum_exp=0.; 
            double sum_cauchy=0.; 
            for (int k=0; k<N[j]; ++k) {
                sum_unif+=rnd.Rannyu(1.,6.);
                sum_exp+=rnd.Exp(1.);
                sum_cauchy+=rnd.Cauchy(0.,1.);
            }
            fout[0]<<sum_unif/N[j]<<lastchar[j];
            fout[1]<<sum_exp/N[j]<<lastchar[j];
            fout[2]<<sum_cauchy/N[j]<<lastchar[j];
        }
    }
 
    return 0;
}
