#include <iostream>
#include <fstream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

#define PI_2 M_PI/2

int main (int argc, char *argv[])
{
    #include "../in/021-conf.inl"

    //Initializing random number generator
    Random rnd("02/in/Primes","02/in/seed.in");

    //Initializing data blocking
    auto m = [&rnd](point<double,1>& p){p[0]=rnd.Rannyu(); return (bool)1;};
    auto f_average = [](point<double,1> p){return PI_2*cos(p[0]*PI_2);};
    auto f_importance_o1 = [](point<double,1> p){ 
        double x=1.-sqrt(1-p[0]);
        return 0.5*PI_2*cos(PI_2*x)/(1.-x);
    };
    array<function<double(point<double,1>)>,2> f = {f_average, f_importance_o1};
    dataBlocks<1,2> integ(STEPS_PER_BLOCK, m, f);

    //Initializing output files
    array<ofstream,2> fout;
    array<string,2> path={"02/out/021-uniform.csv","02/out/021-importance_o1.csv"};
    for(int i=0;i<2;++i) {
        fout[i].open(path[i]);
        if (!fout[i].is_open()){
            cout<<"PROBLEM: unable to open "+path[i]<<endl;
            exit(1);
        }
    }

    //Data blocking
    for(int i=0; i<N_BLOCKS; ++i){
        integ.Measure();
        integ.EvalBlock(fout);
    }

    fout[0].close();
    fout[1].close();

    //lunga: metto in funzione esterna?
    //è giusta l'implementazione dell'accept-reject?
    //Meglio usare gamma o implementare fattoriale? (faccio test)
    //NOT WORKING
    //auto f_importance_oN = [&](int N){
    //    if (N%2==0) N--;
    //    double x,y,f;
    //    do{
    //        x=rnd.Rannyu();
    //        y=rnd.Rannyu(0,PI_2);
    //        f=0;
    //        for (int k=1;k<=N;k+=2) {
    //             f+=pow(-1,(k+1)/2)*PI_2*pow(PI_2*(x-1),k+1)/tgamma(k+1);
    //        }
    //    }while (y>f);
    //    return x;
    //};
    //dataBlocks int_importance_oN(N_BLOCKS, STEPS_PER_BLOCK, bind(f_importance_oN,3));
    //int_importance_oN.ProgressiveAverage("02/out/021-progressive_averages_importance_oN.csv");


    return 0;
}
