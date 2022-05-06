#include "datablocking.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;


dataBlocks :: dataBlocks(int n_blocks, int steps_per_block, function<double()>f) : m_(n_blocks,0.){
    for (double& x : m_) {
        for (int j = 0; j < steps_per_block; j++) 
            x+=f();
        x/=steps_per_block;
    } 
}

dataBlocks :: dataBlocks(int n_blocks, int steps_per_block, function<double()>f, const char* PATH) : m_(n_blocks,0.){
    ofstream fout(PATH);
    if(!fout.is_open()) {
        cerr << "PROBLEM: Unable to open " << PATH << endl;
        exit(1);
    }
    fout<<"N-averaged-data,progressive-average,progressive-error\n";
    double prog_ave = 0; //media progressiva
    double prog_av2 = 0; //media progressiva dei quadrati
    for(int i=0;i<(int)m_.size();++i){
        for (int j = 0; j < steps_per_block; j++) 
            m_[i]+=f();
        m_[i]/=steps_per_block;
        prog_ave+=m_[i];
        prog_av2+=m_[i]*m_[i];
        fout<<(i+1)<<","<<prog_ave/(i+1)<<","<<Error(prog_ave/(i+1),prog_av2/(i+1),i)<<endl; 
    }
}

dataBlocks :: ~dataBlocks(){}

void dataBlocks :: Reset(int n_blocks, int steps_per_block, function<double()>f){
    m_.clear();
    for (int i = 0; i < n_blocks; i++) {
        m_.push_back(0);
        for (int j = 0; j < steps_per_block; j++) 
            m_.back()+=f();
        m_.back()/=steps_per_block;
    } 
}

void dataBlocks :: Map(function<double(double)> f){
    transform(m_.begin(),m_.end(),m_.begin(),f);
}

pair<double,double> dataBlocks :: Stats(bool variance){
    pair<double,double> p(0,0);
    p.first = accumulate(m_.begin(),m_.end(),0.)/m_.size();
    if(variance){
        for(double x : m_) p.second+=(x-p.first)*(x-p.first);
        p.second/=m_.size();
    }
    return p;
}

void dataBlocks :: ProgressiveAverage(const char* PATH){
    ofstream fout(PATH); 
    if(!fout.is_open()) {
        cerr << "PROBLEM: Unable to open " << PATH << endl;
        exit(1);
    }
    fout<<"N-averaged-data,progressive-average,progressive-error\n";

    double prog_ave = 0; //media progressiva
    double prog_av2 = 0; //media progressiva dei quadrati
    for(int i=0;i<(int)m_.size();++i){
        prog_ave+=m_[i];
        prog_av2+=m_[i]*m_[i];
        fout<<(i+1)<<","<<prog_ave/(i+1)<<","<<Error(prog_ave/(i+1),prog_av2/(i+1),i)<<endl; 
    }

    fout.close();
}


double Error(double ave,double av2,int n){
    if(n==0) return 0.;
    else return sqrt((av2-ave*ave)/n);
}
