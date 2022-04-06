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

void RwDiscrete(point<int,3>& p, Random &rnd, int lattice_dim){
    int dir=rnd.Rannyu(0,3); 
    int verse=Sgn(rnd.Rannyu(-1,1)); 
    p[dir]+=verse*lattice_dim;
};

void RwContinuous(point<double,3>& p, Random &rnd, int lattice_dim){
    double theta=acos(1-rnd.Rannyu(0,2));
    double phi=rnd.Rannyu(0,2*M_PI); 

    p[0]+=lattice_dim*sin(theta)*cos(phi);
    p[1]+=lattice_dim*sin(theta)*sin(phi);
    p[2]+=lattice_dim*cos(theta);
};


int main (int argc, char *argv[])
{
    const int N_BLOCKS=100;
    const int RW_PER_BLOCK=1000;
    const int RW_STEPS=100;
    const int LATTICE_DIM=1;

    Random rnd("lib/Random/Primes","lib/Random/seed.in");


    /* Idea:
     * vettore di punti ciascuno evolverà indipendentemente con rws
     * evolvo tutti i random walk di un passo e salvo il risultato
     * in un vettore di datablocks. Evolvo tutti di un altro passo
     * e salvo la distanza in un vettore di datablocks
     * -> ottengo così una misura in datablocsk della distanza ad
     *  ogni passo*/



    //caso discreto
    point<int,3> discrete_rw[RW_PER_BLOCK*N_BLOCKS]; //numero totale di rw
    vector<dataBlocks> discrete_dist;
    discrete_dist.reserve(RW_STEPS);
    for (int i=0; i<RW_STEPS; ++i){
        int k=-1;
        dataBlocks t(N_BLOCKS,RW_PER_BLOCK,[&](){
            k++;
            RwDiscrete(discrete_rw[k],rnd,LATTICE_DIM);
            return discrete_rw[k].Lenght();
        });
        discrete_dist.push_back(t);
    }

    ofstream fout("02/Data/022-discreterw.csv");
    fout<<"average,variance\n";
    for (dataBlocks x : discrete_dist ){
        double ave,var;
        x.Stats(ave,var);
        fout<<ave<<","<<var<<endl;
    }
    fout.close();


    //caso continuo
    point<double,3> continuous_rw[RW_PER_BLOCK*N_BLOCKS]; //numero totale di rw
    vector<dataBlocks> continuous_dist;
    continuous_dist.reserve(RW_STEPS);
    for (int i=0; i<RW_STEPS; ++i){
        int k=-1;
        dataBlocks t(N_BLOCKS,RW_PER_BLOCK,[&](){
            k++;
            RwContinuous(continuous_rw[k],rnd,LATTICE_DIM);
            return continuous_rw[k].Lenght();
        });
        continuous_dist.push_back(t);
    }

    fout.open("02/Data/022-continuousrw.csv");
    fout<<"average,variance\n";
    for (dataBlocks x : continuous_dist ){
        double ave,var;
        x.Stats(ave,var);
        fout<<ave<<","<<var<<endl;
    }
    fout.close();

    
    return 0;
}
