#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Misc/misc.h"
#include "../../lib/Point/point.h"
#include "../../lib/Random/random.h"
#include <array>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;

#include "../in/013-conf-datablocking.inl"
#include "../in/013-conf-experiment.inl"

bool Throw(point<double, 1> &p, Random &rnd) {
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
  Random rnd("01/in/Primes", "01/in/seed.in");

  /* Initializing data blocking */
  array<function<double(point<double, 1>)>, 1> db_eval = {
      [](point<double, 1> p) {
        if (p[0] <= 0)
          return 1;
        else
          return 0;
      }};
  dataBlocks<1, 1> pi_circle(STEPS_PER_BLOCK,
                             bind(Throw, placeholders::_1, rnd), db_eval);
  auto map_val = [](double x) { return 2. * NEEDLE_LENGHT / (x * GRID_SPACE); };
  auto map_err = [](double x, double err) {
    return 2. * err * NEEDLE_LENGHT / (x * x * GRID_SPACE);
  }; // DA SISTEMARE

  /* Initializing output file */
  string path = "01/out/013-pi_progressive_estimate_circle.csv";
  array<ofstream, 1> fout;
  fout[0].open(path);
  FileCheck(fout[0], path);

  /* Data blocking */
  for (int i = 0; i < N_BLOCKS; ++i) {
    pi_circle.Measure();
    pi_circle.Map(0, map_val, map_err);
    pi_circle.EvalBlock(fout);
  }

  fout[0].close();

  return 0;
}
