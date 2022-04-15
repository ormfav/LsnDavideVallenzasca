#include <iostream>
#include <fstream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;


int main (int argc, char *argv[])
{
    #include "../in/021-conf.inl"

    Random rnd("02/in/Primes","02/in/seed.in");
    auto f_average = [&](){return 0.5*M_PI*cos(0.5*M_PI*rnd.Rannyu());};
    dataBlocks int_average(N_BLOCKS, STEPS_PER_BLOCK, f_average,"02/out/021-progressive_averages_average.csv");

    auto f_importance_o1 = [&](){
        double x=1.-sqrt(1-rnd.Rannyu());
        return 0.25*M_PI*cos(0.5*M_PI*x)/(1.-x);
    };
    dataBlocks int_importance_o1(N_BLOCKS, STEPS_PER_BLOCK, f_importance_o1,"02/out/021-progressive_averages_importance_o1.csv");

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
    //int_importance_oN.ProgressiveAverage("02/out/021-progressive_averages_importance_oN.csv");


    return 0;
}
