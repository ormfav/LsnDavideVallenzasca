#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Metropolis/metropolis.h"
#include "../../lib/Misc/misc.h"
#include "../../lib/Point/point.h"
#include "../../lib/Random/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;

/* #include "../config/081-conf.inl" */
#define DELTA 2

#define N_BLOCKS 100
#define STEPS_PER_BLOCK 100000

double AveH(double x, double sigma2, double mu) {
  double x2 = x * x;
  double y = (x2 + mu * mu) / sigma2;

  double kin_part = 0.25 * (1 - y) / (1 + exp(-y)) / sigma2;
  double pot_part = x2 * x2 - 2.5 * x2;

  return kin_part + pot_part;
}

int main(int argc, char *argv[]) {
  double mu = 0.5;
  double sigma2 = 1;
  /* Initializing random number generator */
  Random rnd("lib/Random/Primes", "lib/Random/seed.in");

  /* Initializing data blocking */
  auto psi2_gs = [&mu, &sigma2](point<double, 1> p) {
    return exp(-(p[0] - mu) * (p[0] - mu) / sigma2) +
           exp(-(p[0] + mu) * (p[0] + mu) / sigma2) +
           2 * exp(-0.5 * (p[0] * p[0] + mu * mu) / sigma2);
  };
  array<function<double(point<double, 1>)>, 1> f = {
      [&mu, &sigma2](point<double, 1> p) { return AveH(p[0], sigma2, mu); }};

  array<double, 1> p0 = {0};

  dataBlocks<1, 1> energy(
      STEPS_PER_BLOCK, p0,
      bind(Mrt2Unif<1>, placeholders::_1, psi2_gs, rnd, DELTA), f);

  /* Initializing output files */
  array<ofstream, 1> fout;
  string path = "08/out/081-energy.csv";
  fout[0].open(path);
  FileCheck(fout[0], path);

  /* Data blocking */
  for (int i = 0; i < N_BLOCKS; ++i) {
    cout << "--- Block " << (i + 1) << " ---\n";
    cout << "Acceptance: " << energy.Measure() << endl;
    energy.EvalBlock(fout);
    cout << endl;
  }

  fout[0].close();

  return 0;
}
