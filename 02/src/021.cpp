#include <iostream>
#include <fstream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;


int main (int argc, char *argv[])
{
    const int N_BLOCKS=100; 
    const int STEPS_PER_BLOCK=1000;
    Random rnd("lib/Random/Primes","lib/Random/seed.in");

    auto f_average = [&](){return M_PI/2.*cos(M_PI*rnd.Rannyu()/2.);};
    dataBlocks int_average(N_BLOCKS, STEPS_PER_BLOCK, f_average);
    int_average.ProgressiveAverage("02/Data/021-progressive_averages_average.csv");

    auto f_importance_o1 = [&](){
        double x=1.-sqrt(1-rnd.Rannyu());
        return M_PI/2.*cos(M_PI*x/2.)/(2.*(1.-x));
    };
    dataBlocks int_importance_o1(N_BLOCKS, STEPS_PER_BLOCK, f_importance_o1);
    int_importance_o1.ProgressiveAverage("02/Data/021-progressive_averages_importance_o1.csv");

    //lunga: metto in funzione esterna?
    //è giusta l'implementazione dell'accept-reject?
    //Meglio usare gamma o implementare fattoriale? (faccio test)
    //NOT WORKING
    //auto f_importance_oN = [&](int N){
    //    if (N%2==0) N--;
    //    double x,y,f;
    //    do{
    //        x=rnd.Rannyu();
    //        y=rnd.Rannyu(0,M_PI/2);
    //        f=0;
    //        for (int k=1;k<=N;k+=2) {
    //             f+=pow(-1,(k+1)/2)*M_PI/2*pow(M_PI/2*(x-1),k+1)/tgamma(k+1);
    //        }
    //    }while (y>f);
    //    return x;
    //};
    //dataBlocks int_importance_oN(N_BLOCKS, STEPS_PER_BLOCK, bind(f_importance_oN,3));
    //int_importance_oN.ProgressiveAverage("02/Data/021-progressive_averages_importance_oN.csv");


    return 0;
}
