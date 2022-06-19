#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Metropolis/include/metropolis.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;
using pt1D = point<double, 1>;

#include "../in/081-conf_db.inl"
#include "../in/081-conf_metro.inl"
#include "../include/081.h"

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  array<double, 2> params = {1, 1};

  /* Initializing metropolis */
  auto pdf_ratio = [&params](pt1D q, pt1D p) {
    return Psi2_gs(q, params) / Psi2_gs(p, params);
  };

  mrt2<1> metro(DELTA, pdf_ratio, rnd);

  array<double, 1> p0 = {0};

  string path_equi = "out/081-equi.csv";
  ofstream fout_equi(path_equi);
  FileCheck(fout_equi, path_equi);

  cout << "Acceptance: " << metro.Equilibrate(p0, 500,fout_equi) << endl;

  /* Initializing data blocking */
  auto db_eval = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> energy(STEPS_PER_BLOCK, metro, {db_eval}, p0);

  string path = "out/081-energy.csv";
  array<ofstream, 1> fout;
  fout[0].open(path);
  FileCheck(fout[0], path);

  /* Data blocking */
  do {
    cout << "--- Block " << (energy.CompletedBlocks() + 1) << " ---\n";
    cout << "Acceptance: " << energy.Measure() << endl;
    energy.EvalBlock(fout);
    cout << endl;
  } while (energy.CompletedBlocks() < N_BLOCKS);

  fout[0].close();

  return 0;
}
