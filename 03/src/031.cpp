#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <ranges>

using namespace std;
using pt1D = point<double, 1>;

#include "../in/031-datablocking-conf.inl"
#include "../in/031-options-conf.inl"

int main(int argc, char *argv[]) {
  /* Setting random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Setting data blocking */
  // Eval option prices
  array<function<double(pt1D)>, 2> db_eval = {
      [](pt1D p) {
        return p[0] > STRIKE_PRICE
                   ? exp(-INTREST_RATE * T) * (p[0] - STRIKE_PRICE)
                   : 0;
      },
      [](pt1D p) {
        return p[0] < STRIKE_PRICE
                   ? exp(-INTREST_RATE * T) * (STRIKE_PRICE - p[0])
                   : 0;
      }};
  // Direct move
  auto db_move_drct = [&rnd](pt1D &p) {
    p[0] = S0 * exp((INTREST_RATE - 0.5 * SIGMA * SIGMA) * T +
                    SIGMA * sqrt(T) * rnd.Gauss(0, 1));
    return 1;
  };
  // Discrete move
  auto db_move_disc = [&rnd](pt1D &p) {
    double t = (double)T / N_INTERVALS;
    p[0] = S0;
    for (size_t i = 0; i < N_INTERVALS; ++i) {
      double z = rnd.Gauss(0, 1);
      p[0] *=
          exp((INTREST_RATE - 0.5 * SIGMA * SIGMA) * t + SIGMA * sqrt(t) * z);
    }
    return 1;
  };
  dataBlocks<1, 2> opt_drct(STEPS_PER_BLOCK, db_move_drct, db_eval);
  dataBlocks<1, 2> opt_disc(STEPS_PER_BLOCK, db_move_disc, db_eval);

  /* Setting output files */
  array<ofstream, 2> fout_drct;
  array<ofstream, 2> fout_disc;
  array<string, 2> path_drct = {"out/031-callopt_direct.csv",
                                "out/031-putopt_direct.csv"};
  array<string, 2> path_disc = {"out/031-callopt_discrete.csv",
                                "out/031-putopt_discrete.csv"};
  for (size_t i = 0; i < 2; ++i) {
    fout_drct[i].open(path_drct[i]);
    FileCheck(fout_drct[i], path_drct[i]);
    fout_disc[i].open(path_disc[i]);
    FileCheck(fout_disc[i], path_disc[i]);
  }

  /* Data blocking */
  do {
    opt_drct.Measure();
    opt_disc.Measure();
    opt_drct.EvalBlock(fout_drct);
    opt_disc.EvalBlock(fout_disc);
  } while (opt_disc.CompletedBlocks() < N_BLOCKS);

  for (size_t i = 0; i < 2; ++i) {
    fout_drct[i].close();
    fout_disc[i].close();
  }

  return 0;
}
