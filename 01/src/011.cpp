#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Misc/misc.h"
#include "../../lib/Random/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
/*------------------------------*/
/* Evaluatng Average - Variance */
/*------------------------------*/
#include "../in/011-ave_var-conf.inl"

  /* Initializing random number generator */
  Random rnd("01/in/Primes", "01/in/seed.in");

  /* Initializing data blocking */
  auto db_move = [&rnd](point<double, 1> &p) {
    p[0] = rnd.Rannyu();
    return (bool)1;
  };
  array<function<double(point<double, 1>)>, 2> db_eval = {
      [](point<double, 1> p) { return p[0]; },
      [](point<double, 1> p) { return (p[0] - 0.5) * (p[0] - 0.5); }};
  dataBlocks<1, 2> ave_var(STEPS_PER_BLOCK, db_move, db_eval);

  /* initializing output files */
  array<ofstream, 2> fout_ave_var;
  array<string, 2> path = {"01/out/011-mean_value.csv",
                           "01/out/011-variance.csv"};
  for (int i = 0; i < 2; ++i) {
    fout_ave_var[i].open(path[i]);
    FileCheck(fout_ave_var[i], path[i]);
  }

  /* Data blocking */
  for (int i = 0; i < N_BLOCKS; ++i) {
    ave_var.Measure();
    ave_var.EvalBlock(fout_ave_var);
  }

  fout_ave_var[0].close();
  fout_ave_var[1].close();

/*-----------*/
/* chi2 test */
/*-----------*/
#include "../in/011-chi2-conf.inl"
  double expected_value = (double)THROWS / N_INTERVALS;

  ofstream fout_chi2("01/out/011-chi2.dat");
  FileCheck(fout_chi2, "01/out/011-chi2.dat");

  for (int i = 0; i < N_TESTS; ++i) {
    array<int, N_INTERVALS> hyst = {0};
    double chi2 = 0;
    for (int j = 0; j < THROWS; ++j) {
      int t = rnd.Rannyu() * N_INTERVALS;
      hyst[t]++;
    }
    for (int t : hyst)
      chi2 += (t - expected_value) * (t - expected_value);
    fout_chi2 << chi2 / expected_value << endl;
  }
  fout_chi2.close();

  return 0;
}
