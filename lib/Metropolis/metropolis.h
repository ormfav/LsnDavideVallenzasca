#ifndef __Metropolis__
#define __Metropolis__

#include <functional>
#include "../Random/random.h"
#include "../Point/point.h"

using namespace std;

template<int DIM> bool Mrt2Unif(point<double,DIM>&, function<double(point<double,DIM>)>, Random&, double);
template<int DIM> bool Mrt2Gauss(point<double,DIM>&, function<double(point<double,DIM>)>, Random&, double );


template<int DIM> 
bool Mrt2Unif(point<double,DIM>& p, function<double(point<double,DIM>)> pdf, Random& rnd, double delta){
    auto m = [&rnd,&delta](point<double,DIM>& p){for(double& x : p.ToArray())x+=rnd.Rannyu(-delta,delta); return 1;};
    point<double,DIM> q(p.ToArray(),m);
    q.Move();
    double test = pdf(q)/pdf(p);
	if (test >= 1.){
        p.ToArray()=q.ToArray();
        return 1;
    } else if(rnd.Rannyu() < test){
        p.ToArray()=q.ToArray();
        return 1;
    }

    return 0;
}

template<int DIM> 
bool Mrt2Gauss(point<double,DIM>& p, function<double(point<double,DIM>)> pdf, Random& rnd, double sigma){
    auto m = [&rnd,&sigma](point<double,DIM>& p){for(double& x : p.ToArray())x+=rnd.Gauss(0,sigma); return 1;};
    point<double,DIM> q(p.ToArray(),m);
    q.Move();
    double test = pdf(q)/pdf(p);
    if (test >= 1.){
        p.ToArray()=q.ToArray();
        return 1;
    } else if(rnd.Rannyu() < test){
        p.ToArray()=q.ToArray();
        return 1;
    }

    return 0;
}

#endif
