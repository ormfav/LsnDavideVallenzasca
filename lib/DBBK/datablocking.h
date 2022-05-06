#ifndef __DataBlocking__
#define __DataBlocking__

//DA RIVEDERE ALLA FINE CON QUELLO CHE USO DAVVERO

#include <vector>
#include <utility> //pair
#include <functional>
#include "../Random/random.h"

class dataBlocks
{
public:
    dataBlocks(int n_blocks, int steps_per_block, std::function<double()>);
    //eval ProgressiveAverages on construction
    dataBlocks(int n_blocks, int steps_per_block, std::function<double()>, const char* PATH);
    ~dataBlocks();
    
    void Reset(int n_blocks, int steps_per_block, std::function<double()>);
    void Eval(int n_blocks, int steps_per_block);
    void Map(std::function<double(double)>);
    std::pair<double,double> Stats(bool variance);
    void ProgressiveAverage(const char* PATH);

private:
    std::vector<double> m_;
};

double Error(double ave,double av2,int n);

#endif
