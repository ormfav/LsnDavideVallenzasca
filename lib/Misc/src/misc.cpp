#include "../include/misc.h"

double Error(double sum,double sum2,int n)
{
    return sqrt(fabs(sum2/(double)n - pow(sum/(double)n,2))/(double)n);
}

void FileCheck(ofstream& file, string path){
    if(!file){
        cerr<<"ERROR: unable to open file "+path<<endl;
        exit(1);
    }
}

void FileCheck(ifstream& file, string path){
    if(!file){
        cerr<<"ERROR: unable to open file "+path<<endl;
        exit(1);
    }
}
