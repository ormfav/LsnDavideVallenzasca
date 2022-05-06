#ifndef __DataBlocking__
#define __DataBlocking__

#include <array>
#include <functional>
#include "../Random/random.h"
#include "../Point/point.h"

using namespace std;

template<int PARAMS, int PROPS> class dataBlocks
{
public:
    dataBlocks(function<point<double,PARAMS>()> move, function<array<double,PROPS>(point<double,PARAMS>)> eval_props);
    double Move();

private:
    point<double,PARAMS> params_;
    function<array<double,PROPS>(point<double,PARAMS>)> eval_props_;


};

double Error(double ave,double av2,int n);

#endif
