/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "../include/random.h"

using namespace std;

Random :: Random(){}

Random :: Random(const char *PRIMES, const char *SEEDIN){
    int seed[4];
    int p1, p2;
    ifstream Primes(PRIMES);
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else {
      cerr << "PROBLEM: Unable to open Primes" << endl;
      exit(-1);
    }
    Primes.close();

    ifstream input(SEEDIN);
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
           input >> property;
           if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else {
      cerr << "PROBLEM: Unable to open seed.in" << endl;
      exit(-1);
    }

}

Random :: ~Random(){}

void Random :: SaveSeed(const char *PATH){
   ofstream WriteSeed;
   WriteSeed.open(PATH);
   if (WriteSeed.is_open()){
      WriteSeed << l1 << " " << l2 << " " << l3 << " " << l4 << endl;;
   } else cerr << "PROBLEM: Unable to open " << PATH << endl;
  WriteSeed.close();
  return;
}

double Random :: Exp(double rate) {
	return -log(1-Rannyu()) / rate;
}

double Random :: Cauchy(double loc, double scale) {
	return loc + scale * tan(M_PI * (Rannyu() - 0.5));
}

double Random :: Gauss(double mean, double sigma) {
   double s=Rannyu();
   double t=Rannyu();
   double x=sqrt(-2.*log(1.-s))*cos(2.*M_PI*t);
   return mean + x * sigma;
}

double Random :: Rannyu(double min, double max){
   return min+(max-min)*Rannyu();
}

double Random :: Rannyu(){
  const double twom12=0.000244140625;
  int i1,i2,i3,i4;
  double r;

  i1 = l1*m4 + l2*m3 + l3*m2 + l4*m1 + n1;
  i2 = l2*m4 + l3*m3 + l4*m2 + n2;
  i3 = l3*m4 + l4*m3 + n3;
  i4 = l4*m4 + n4;
  l4 = i4%4096;
  i3 = i3 + i4/4096;
  l3 = i3%4096;
  i2 = i2 + i3/4096;
  l2 = i2%4096;
  l1 = (i1 + i2/4096)%4096;
  r=twom12*(l1+twom12*(l2+twom12*(l3+twom12*(l4))));

  return r;
}

void Random :: SetRandom(int * s, int p1, int p2){
  m1 = 502;
  m2 = 1521;
  m3 = 4071;
  m4 = 2107;
  l1 = s[0];
  l2 = s[1];
  l3 = s[2];
  l4 = s[3];
  n1 = 0;
  n2 = 0;
  n3 = p1;
  n4 = p2;

  return;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/