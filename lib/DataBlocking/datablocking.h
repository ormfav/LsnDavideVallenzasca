#ifndef __DataBlocking__
#define __DataBlocking__

#include <array>
#include <functional>
#include <fstream>
#include "../Random/random.h"
#include "../Point/point.h"

#include <iostream>

using namespace std;

template<int PARAMS, int PROPS> class dataBlocks
{
public:
    dataBlocks(int steps, function<bool(array<double,PARAMS>&)> move, array<function<double(point<double,PARAMS>)>,PROPS> eval_props);

    double Measure();
    void EvalBlock(array<ofstream,PROPS>&);
    void EvalBlock();

    array<double,PROPS> GetBlkAve();
    array<double,PROPS> GetBlkErr();
    array<double,PROPS> GetPrgAve();
    array<double,PROPS> GetPrgErr();

private:
    const int STEPS_; //steps 
    point<double,PARAMS> params_;

    array<function<double(point<double,PARAMS>)>,PROPS> eval_props_;
        
    //value and error of the single block
    array<double,PROPS> blk_ave_; 
    array<double,PROPS> blk_err_; 
    //progressive average and error of the blocks
    array<double,PROPS> prg_ave_; 
    array<double,PROPS> prg_av2_; //aux: needed to evaluate progressive error
    array<double,PROPS> prg_err_; 

    int nblk_; 
};

double Error(double sum,double sum2,int n)
{
    return sqrt(fabs(sum2/(double)n - pow(sum/(double)n,2))/(double)n);
}

template<int PARAMS, int PROPS> dataBlocks<PARAMS,PROPS>::dataBlocks(int steps, function<bool(array<double,PARAMS>&)> move, array<function<double(point<double,PARAMS>)>,PROPS> eval_props) :
STEPS_(steps),
params_(move)
{
    eval_props_=eval_props;
    blk_ave_={0};
    blk_err_={0};
    prg_ave_={0};
    prg_av2_={0}; 
    prg_err_={0};
    nblk_=0;
}

template<int PARAMS, int PROPS> double dataBlocks<PARAMS,PROPS>::Measure(){
    array<double,PROPS> av2={0};
    for(double& x : blk_ave_) x=0;

    for(int i=0; i<STEPS_; ++i){
        params_.Move();
        for(int j=0; j<PROPS; ++j) {
            double aux=eval_props_[j](params_);
            blk_ave_[j]+=aux; 
            av2[j]+=aux*aux;
        }
    }

    for(int i=0; i<PROPS; ++i) blk_err_[i]=Error(blk_ave_[i],av2[i],STEPS_);
    for(double& x : blk_ave_) x/=STEPS_;
    return params_.Rate();
}

template<int PARAMS, int PROPS> void dataBlocks<PARAMS,PROPS>::EvalBlock(array<ofstream,PROPS>& fout){
    if(!nblk_){ //undefined behaviour?
        for (ofstream& x : fout) x << "block_number,block_average,block_error,progressive_average,progressive_error\n" ;
    }
    nblk_++;
    for(int i=0;i<PROPS;++i) {
        prg_ave_[i]+=blk_ave_[i];
        prg_av2_[i]+=blk_ave_[i]*blk_ave_[i];
        prg_err_[i]=Error(prg_ave_[i], prg_av2_[i], nblk_);
        fout[i] << nblk_ << "," << blk_ave_[i] << "," << blk_err_[i] << "," << prg_ave_[i]/nblk_ << "," << prg_err_[i] << endl;
    }
}

template<int PARAMS, int PROPS> void dataBlocks<PARAMS,PROPS>::EvalBlock(){
    nblk_++;
    for(int i=0;i<PROPS;++i) {
        prg_ave_[i]+=blk_ave_[i];
        prg_av2_[i]+=blk_ave_[i]*blk_ave_[i];
        prg_err_[i]=Error(prg_ave_[i], prg_av2_[i], nblk_);
    }
}

template<int PARAMS, int PROPS> array<double,PROPS> dataBlocks<PARAMS,PROPS>::GetBlkAve(){
    return blk_ave_;
}

template<int PARAMS, int PROPS> array<double,PROPS> dataBlocks<PARAMS,PROPS>::GetBlkErr(){
    return blk_err_;
}

template<int PARAMS, int PROPS> array<double,PROPS> dataBlocks<PARAMS,PROPS>::GetPrgAve(){
    array<double,PROPS> out=prg_ave_;
    for(double& x : out) x/=nblk_;
    return out;
}

template<int PARAMS, int PROPS> array<double,PROPS> dataBlocks<PARAMS,PROPS>::GetPrgErr(){
    return prg_err_;
}

#endif
