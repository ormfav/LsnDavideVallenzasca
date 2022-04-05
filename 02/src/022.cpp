#include <functional>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "../../lib/Random/random.h"
#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Point/point.h"

using namespace std;

int Sgn(double x){
    return (x>=0)-(x<0);
}

point<int,3> Rw_discrete(Random &rnd, point<int,3>p, int lattice_dim){
    int direction=rnd.Rannyu(0,3); 
    int verse=Sgn(rnd.Rannyu(-1,1)); 
        p.GetPoint()[direction]+=verse*lattice_dim;
    return p;
};

int main (int argc, char *argv[])
{
    const int N_BLOCKS=100;
    const int RW_PER_BLOCK=1000;
    const int RW_STEPS=100;
    const int LATTICE_DIM=1;

    Random rnd("lib/Random/Primes","lib/Random/seed.in");

    //caso discreto
    point<int,3> rws[RW_PER_BLOCK*N_BLOCKS]; //numero totale di rw
    vector<dataBlocks> discrete;
    discrete.reserve(RW_STEPS);
    for (int i=0; i<RW_STEPS; ++i){
        int k=-1;
        dataBlocks t(N_BLOCKS,RW_PER_BLOCK,[&](){
            k++;
            rws[k].Evolve(bind(Rw_discrete,rnd,placeholders::_1,LATTICE_DIM));
            return rws[k].Lenght();
            });
        discrete.push_back(t);
    }



    ////caso continuo
    //for (int i=1; i<N_STEPS; ++i) {
    //    double theta=rnd.Rannyu(0,M_PI); 
    //    double phi=rnd.Rannyu(0,2*M_PI); 

    //    coord_continue[i][0]+=coord_continue[i-1][0]+LATTICE_DIM*sin(theta)*cos(phi);
    //    coord_continue[i][1]+=coord_continue[i-1][1]+LATTICE_DIM*sin(theta)*cos(phi);
    //    coord_continue[i][2]+=coord_continue[i-1][2]+LATTICE_DIM*cos(theta);
    //}
    //
    



    return 0;
}
