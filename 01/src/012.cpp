#include <iostream>
#include <fstream>
#include "../../lib/Random/random.h"

using namespace std;
//TODO: documento e rivedo nome variabili
//riorganizzo ciclo? magari funzione esterna?

int main (int argc, char *argv[])
{
    #include "../in/012-conf.inl"
    int N[4] = {1,2,10,100};//da rinominare?
    char lastchar[4]={',', ',', ',', '\n'}; //formatting output files

    Random rnd("lib/Random/Primes","lib/Random/seed.in");
 
    ofstream fout_unif("01/out/012-unif.csv");
    fout_unif<<"N=1,N=2,N=10,N=100\n";
    ofstream fout_exp("01/out/012-exp.csv");
    fout_exp<<"N=1,N=2,N=10,N=100\n";
    ofstream fout_cauchy("01/out/012-cauchy.csv"); 
    fout_cauchy<<"N=1,N=2,N=10,N=100\n";

    for (int i=0; i<THROWS; ++i) {
        for(int j=0; j<4; ++j){
            double sum_unif=0.; 
            double sum_exp=0.; 
            double sum_cauchy=0.; 
            for (int k=0; k<N[j]; ++k) {
                sum_unif+=rnd.Rannyu(1.,6.);
                sum_exp+=rnd.Exp(1.);
                sum_cauchy+=rnd.Cauchy(0.,1.);
            }
            fout_unif<<sum_unif/N[j]<<lastchar[j];
            fout_exp<<sum_exp/N[j]<<lastchar[j];
            fout_cauchy<<sum_cauchy/N[j]<<lastchar[j];
        }
    }
 
    return 0;
}
