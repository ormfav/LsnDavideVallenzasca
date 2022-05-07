//TODO: riesco a implementare il metodo col punto fisso usando la
//classe?? 
#include <functional>
#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"

using namespace std;

#include "../in/013-conf.inl"

bool Throw (array<double,1>& cont, Random& rnd){
    double x;
    double y;
    for(int i=0; i<STEPS_PER_BLOCK; ++i){
    double needle_mid=rnd.Rannyu()*GRID_SPACE*0.5;
        do{
            x=rnd.Rannyu();
            y=rnd.Rannyu();
        } while(x*x+y*y>1);
        double needle_proj_lenght=NEEDLE_LENGHT*x/hypot(x,y); 
        if(needle_mid-0.5*needle_proj_lenght<=0 )
            cont[0]+=1;
    }
    return 1;
}

double Eval(point<double,1> p){
    double x=2.*NEEDLE_LENGHT/(p[0]*GRID_SPACE);
    p.Reset();
    return x;
}


int main (int argc, char *argv[])
{
    Random rnd("01/in/Primes","01/in/seed.in");

    array<function<double(point<double,1>)>,1> f = {Eval};
    dataBlocks<1,1> pi_circle(1,bind(Throw,placeholders::_1,rnd),f);

    string path ="01/out/013-pi_progressive_estimate_circle.csv";
    array<ofstream,1> fout;
    fout[0].open(path);
    if(!fout[0].is_open())
        cout<< "PROBLEM: unable to open "+path<<endl;

    for(int i=0; i<N_BLOCKS; ++i){
        pi_circle.Measure();
        cout<<pi_circle.GetBlkAve()[0]<<endl;
        pi_circle.EvalBlock(fout);
    }

    fout[0].close();


    return 0;
}
