#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Random/include/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;
using pt1D = point<double, 1>;

int main(int argc, char *argv[]) {
/*------------------------------*/
/* Evaluatng Average - Variance */
/*------------------------------*/
#include "../in/011-ave_var-conf.inl"

  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Initializing data blocking */
  auto db_move = [&rnd](pt1D &p) {
    p[0] = rnd.Rannyu();
    return (bool)1;
  };
  array<function<double(pt1D)>, 2> db_eval = {
      [](pt1D p) { return p[0]; },
      [](pt1D p) { return (p[0] - 0.5) * (p[0] - 0.5); }};
  dataBlocks<1, 2> ave_var(STEPS_PER_BLOCK, db_move, db_eval);

  /* initializing output files */
  array<ofstream, 2> fout_ave_var;
  array<string, 2> path = {"out/011-mean_value.csv", "out/011-variance.csv"};
  for (int i = 0; i < 2; ++i) {
    fout_ave_var[i].open(path[i]);
    FileCheck(fout_ave_var[i], path[i]);
  }

  /* Data blocking */
  do {
    ave_var.Measure();
    ave_var.EvalBlock(fout_ave_var);
  } while (ave_var.CompletedBlocks() < N_BLOCKS);

  for (auto &x : fout_ave_var)
    x.close();

/*-----------*/
/* chi2 test */
/*-----------*/
#include "../in/011-chi2-conf.inl"
  double expected_value = (double)THROWS / N_INTERVALS;

  ofstream fout_chi2("out/011-chi2.dat");
  FileCheck(fout_chi2, "out/011-chi2.dat");

  for (size_t i = 0; i < N_TESTS; ++i) {
    array<int, N_INTERVALS> hist = {0};
    double chi2 = 0;
    for (size_t j = 0; j < THROWS; ++j) {
      int t = rnd.Rannyu() * N_INTERVALS;
      hist[t]++;
    }
    for (int t : hist)
      chi2 += (t - expected_value) * (t - expected_value);
    fout_chi2 << chi2 / expected_value << endl;
  }
  fout_chi2.close();

  return 0;
}
