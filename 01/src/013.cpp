//TODO: riesco a implementare il metodo col punto fisso usando la
//classe?? 
#include <iostream>
#include <fstream>
#include <cmath>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"

using namespace std;

double CircleMethod(Random& rnd, double needle_lenght, double grid_space){
    double needle_mid=rnd.Rannyu(0, grid_space/2.);
    double x;
    double y;
    do{
        x=rnd.Rannyu();
        y=rnd.Rannyu();
    } while(x*x+y*y>1);
    double needle_proj_lenght=needle_lenght*x/hypot(x,y); //posso semplificare? 
    if( needle_mid-0.5*needle_proj_lenght<=0 )
        return 1;
    return 0;
}

int main (int argc, char *argv[])
{
    #include "../config/013-conf.inl"
    Random rnd("lib/Random/Primes","lib/Random/seed.in");

    dataBlocks pi_circle(N_BLOCKS,STEPS_PER_BLOCK,bind(CircleMethod,rnd,NEEDLE_LENGHT,GRID_SPACE));
    auto fp = [](double x){return 2.*NEEDLE_LENGHT/(x*GRID_SPACE);};
    pi_circle.Map(fp);
    pi_circle.ProgressiveAverage("01/Data/013-pi_progressive_estimate_circle.csv");
 
    return 0;
}
