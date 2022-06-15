#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;
using pt1D = point<double, 1>;

#include "../in/013-conf-datablocking.inl"
#include "../in/013-conf-experiment.inl"

bool Throw(pt1D &p, Random &rnd) {
  double x;
  double y;
  double needle_mid = rnd.Rannyu() * GRID_SPACE * 0.5;
  do {
    x = rnd.Rannyu();
    y = rnd.Rannyu();
  } while (x * x + y * y > 1);
  // projeting the needle onto vertical line
  p[0] = NEEDLE_LENGHT * x / hypot(x, y);

  p[0] = needle_mid - 0.5 * p[0];

  return 1;
}

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Initializing data blocking */
  array<function<double(pt1D)>, 1> db_eval = {[](pt1D p) {
    if (p[0] <= 0)
      return 1;
    else
      return 0;
  }};

  dataBlocks<1, 1> pi(STEPS_PER_BLOCK, bind(Throw, placeholders::_1, rnd),
                      db_eval);
  auto map = [](double x) { return 2. * NEEDLE_LENGHT / (x * GRID_SPACE); };

  /* Initializing output file */
  string path = "out/013-pi_progressive_estimate.csv";
  array<ofstream, 1> fout;
  fout[0].open(path);
  FileCheck(fout[0], path);

  /* Data blocking */
  for (int i = 0; i < N_BLOCKS; ++i) {
    pi.Measure();
    pi.Map(0, map);
    pi.EvalBlock(fout);
  }

  fout[0].close();

  return 0;
}
