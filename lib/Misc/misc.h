#ifndef __Misc__
#define __Misc__

#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <functional>
#include "../Point/point.h"
#include "../Random/random.h"

using namespace std;


double Error(double sum,double sum2,int n);
void FileCheck(ofstream&, string);
template<int N> bool SymMetropolis(point<double,N>&, function<double(point<double,N>)>, Random& );


double Error(double sum,double sum2,int n)
{
    return sqrt(fabs(sum2/(double)n - pow(sum/(double)n,2))/(double)n);
}

void FileCheck(ofstream& fout, string path){
    if(!fout.is_open()){
        cerr<<"ERROR: unable to open file "+path<<endl;
        exit(1);
    }
}

template<int DIM> 
bool Mrt2Unif(point<double,DIM>& p, function<double(point<double,DIM>)> pdf, Random& rnd, double delta){
    auto m = [&rnd,&delta](point<double,DIM>& p){for(int i=0;i<DIM;++i)p[i]+=rnd.Rannyu(-delta,delta); return 1;};
    point<double,DIM> q(p.ToArray(),m);
    q.Move();
    double test = pdf(q)/pdf(p);
	if (test >= 1. || rnd.Rannyu() < test) {
        p.ToArray()=q.ToArray();
        return 1;
    }
    return 0;
}

template<int DIM> 
bool Mrt2Gauss(point<double,DIM>& p, function<double(point<double,DIM>)> pdf, Random& rnd, double sigma){
    auto m = [&rnd,&sigma](point<double,DIM>& p){for(int i=0;i<DIM;++i)p[i]+=rnd.Gauss(0,sigma); return 1;};
    point<double,DIM> q(p.ToArray(),m);
    q.Move();
    double test = pdf(q)/pdf(p);
	if (test >= 1. || rnd.Rannyu() < test) {
        p.ToArray()=q.ToArray();
        return 1;
    }
    return 0;
}

/* template<int DIM>  */
/* bool SymMetropolis(point<double,DIM>& x, function<double(point<double,DIM>)> pdf, Random& rnd){ */
/*     point<double,DIM> y=x; */
/*     y.Move(); */
/*     double test = pdf(y)/pdf(x); */
/* 	if (test >= 1. || rnd.Rannyu() < test) { */
/*         x=y; */
/*         return 1; */
/*     } */
/*     return 0; */
/* } */

#endif
