#ifndef __DataBlocking__
#define __DataBlocking__

//DA RIVEDERE ALLA FINE CON QUELLO CHE USO DAVVERO

#include <vector>//Posso usare array?
#include <fstream>
#include <cmath>
#include <functional>
#include "../Random/random.h"

class dataBlocks
{
public:
    dataBlocks(int n_blocks, int steps_per_block, std::function<double()>);
    ~dataBlocks();
    
    void Reset(int n_blocks, int steps_per_block, std::function<double()>);
    void Eval(int n_blocks, int steps_per_block);
    void Map(std::function<double(double)>);
    double Average();
    double Variance();
    void ProgressiveAverage(const char* PATH);

private:
    std::vector<double> m_;
    std::function<double()> fp_eval_;
};

double Error(double ave,double av2,int n);

#endif
