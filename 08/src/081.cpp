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
#define DELTA 3

#define N_BLOCKS 100
#define STEPS_PER_BLOCK 100000

double Integrand(point<double, 1> p, array<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double x = p[0];
  double x2 = x * x;

  double kin = mu * mu + x2 - 2 * mu * x * tanh(mu * x / sigma2) - sigma2;
  double pot = x2 * x2 - 2.5 * x2;

  return pot - 0.5 * kin / (sigma2 * sigma2);
}

double Psi2_gs(point<double, 1> p, array<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double psi = exp(-0.5 * (p[0] - mu) * (p[0] - mu) / sigma2) +
               exp(-0.5 * (p[0] + mu) * (p[0] + mu) / sigma2);
  return psi * psi;
};

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("lib/Random/Primes", "lib/Random/seed.in");

  array<double, 2> params = {1,1};

  /* Initializing data blocking */
  function<double(point<double, 1>)> pdf =
      bind(Psi2_gs, placeholders::_1, params);
  auto m = bind(Mrt2Unif<1>, placeholders::_1, pdf, rnd, DELTA);
  auto f = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> energy(STEPS_PER_BLOCK, {0}, m, {f});

  string path = "08/out/081-energy.csv";
  array<ofstream, 1> fout;
  fout[0].open(path);
  FileCheck(fout[0], path);

  /* Data blocking */
  for (size_t idb = 0; idb < N_BLOCKS; ++idb) {

    cout << "--- Block " << (idb + 1) << " ---\n";
    cout << "Acceptance: " << energy.Measure() << endl;
    energy.EvalBlock(fout);
    cout << endl;
  }

  fout[0].close();

  return 0;
}
