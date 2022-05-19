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


double Error(double sum,double sum2,int n)
{
    return sqrt(fabs(sum2/(double)n - pow(sum/(double)n,2))/(double)n);
}

void FileCheck(ofstream& fout, string path){
    if(!fout){
        cerr<<"ERROR: unable to open file "+path<<endl;
        exit(1);
    }
}

#endif
