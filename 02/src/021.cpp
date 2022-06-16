#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Random/include/random.h"
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;
using pt1D = point<double, 1>;

#include "../in/021-conf.inl"
#define PI_2 M_PI * 0.5

int main(int argc, char *argv[]) {

  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Initializing data blocking */
  auto db_move = [&rnd](pt1D &p) {
    p[0] = rnd.Rannyu();
    return (bool)1;
  };
  array<function<double(pt1D)>, 2> db_eval = {
      [](pt1D p) { return PI_2 * cos(p[0] * PI_2); },
      [](pt1D p) {
        /* Inversion of the cumulative */
        double x = 1. - sqrt(1 - p[0]);
        /* Actual evaluation of the integrand */
        return 0.5 * PI_2 * cos(PI_2 * x) / (1. - x);
      }};

  dataBlocks<1, 2> integ(STEPS_PER_BLOCK, db_move, db_eval);

  /* Initializing output files */
  array<ofstream, 2> fout;
  array<string, 2> path = {"out/021-uniform.csv", "out/021-importance.csv"};
  for (size_t i = 0; i < 2; ++i) {
    fout[i].open(path[i]);
    FileCheck(fout[i], path[i]);
  }

  /* Data blocking */
  do {
    integ.Measure();
    integ.EvalBlock(fout);
  } while (integ.CompletedBlocks() < N_BLOCKS);

  fout[0].close();
  fout[1].close();

  return 0;
}
