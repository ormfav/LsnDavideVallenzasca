#include "../../lib/Misc/include/misc.h"
#include "../../lib/Random/include/random.h"
#include <array>
#include <fstream>
#include <iostream>

using namespace std;

#include "../in/012-conf.inl"

int main(int argc, char *argv[]) {
  array<size_t, 4> N = {1, 2, 10, 100};
  array<char, 4> lastchar = {',', ',', ',', '\n'}; // formatting output files
  /* Computing increments */
  int prev = 0;
  for (auto &x : N) {
    x -= prev;
    prev += x;
  }


  Random rnd("in/Primes", "in/seed.in");

  array<ofstream, 3> fout;
  array<string, 3> path = {"out/012-unif.csv", "out/012-exp.csv",
                           "out/012-cauchy.csv"};

  for (size_t i = 0; i < 3; i++) {
    fout[i].open(path[i]);
    FileCheck(fout[i], path[i]);
    fout[i] << "N=1,N=2,N=10,N=100\n";
  }

  for (size_t i = 0; i < T; ++i) {
    double sum_unif = 0.;
    double sum_exp = 0.;
    double sum_cauchy = 0.;
    size_t tot_throws= 0;
    for (size_t j = 0; j < N.size(); ++j) {
      for (size_t k = 0; k < N[j]; ++k) {
        sum_unif += rnd.Rannyu();
        sum_exp += rnd.Exp(1.);
        sum_cauchy += rnd.Cauchy(0., 1.);
      }
      tot_throws+=N[j];
      fout[0] << sum_unif / tot_throws << lastchar[j];
      fout[1] << sum_exp / tot_throws << lastchar[j];
      fout[2] << sum_cauchy / tot_throws << lastchar[j];
    }
  }

  return 0;
}
