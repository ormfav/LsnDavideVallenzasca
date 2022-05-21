#include "../../lib/DataBlocking/datablocking.h"
#include "../../lib/Misc/misc.h"
#include "../../lib/Point/point.h"
#include "../../lib/Random/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>

using namespace std;

int Sgn(double x) { return (x >= 0) - (x < 0); }

void RwDiscrete(point<double, 3> &p, Random &rnd, double lattice_dim) {
  int dir = rnd.Rannyu(0, 3);
  int verse = Sgn(rnd.Rannyu(-1, 1));
  p[dir] += verse * lattice_dim;
};

void RwContinuous(point<double, 3> &p, Random &rnd, double lattice_dim) {
  double theta = acos(1 - rnd.Rannyu(0, 2));
  double phi = rnd.Rannyu(0, 2 * M_PI);

  p[0] += lattice_dim * sin(theta) * cos(phi);
  p[1] += lattice_dim * sin(theta) * sin(phi);
  p[2] += lattice_dim * cos(theta);
};

#include "../in/022-conf.inl"
int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("02/in/Primes", "02/in/seed.in");

  /* functions */
  queue<array<double, 3>> store_rw_pos;
  auto m = [&rnd, &store_rw_pos](
               point<double, 3> &p,
               function<void(point<double, 3> &, Random &, double)> rw_move) {
    p.Jump(store_rw_pos.front());
    store_rw_pos.pop();
    rw_move(p, rnd, LATTICE_DIM);
    store_rw_pos.push(p.ToArray());
    return 1;
  };
  array<function<double(point<double, 3>)>, 1> f = {
      [](point<double, 3> p) { return p.Lenght2(); }};

  /* -------- */
  /* Discrete  */
  /* -------- */

  /* Initializing data blocking */
  dataBlocks<3, 1> r_discrete(RW_PER_BLOCK,
                              bind(m, placeholders::_1, RwDiscrete), f);
  for (int i = 0; i < RW_PER_BLOCK * N_BLOCKS; ++i)
    store_rw_pos.push({0});

  /* Initializing output file */
  string path = "02/out/022-discreterw.csv";
  ofstream fout(path);
  FileCheck(fout, path);
  fout << "step,r_ave,r_err\n";

  /* Data blocking  */
  for (int i = 0; i < RW_STEPS; ++i) {
    for (int j = 0; j < N_BLOCKS; ++j) {
      r_discrete.Measure();
      r_discrete.EvalBlock();
    }
    double val = sqrt(r_discrete.GetPrgAve()[0]);
    double err = 0.5 * r_discrete.GetPrgErr()[0] / val;
    fout << (i + 1) << "," << val << "," << err << endl;
  }

  fout.close();

  /* ---------- */
  /* Continuous */
  /* ---------- */

  /* Initializing data blocking */
  dataBlocks<3, 1> r_continuous(RW_PER_BLOCK,
                                bind(m, placeholders::_1, RwContinuous), f);
  for (int i = 0; i < RW_PER_BLOCK * N_BLOCKS; ++i) {
    store_rw_pos.pop();
    store_rw_pos.push({0});
  }

  /* Initializing output file */
  path = "02/out/022-continuousrw.csv";
  fout.open(path);
  FileCheck(fout, path);
  fout << "step,r_ave,r_err\n";

  /* Data blocking  */
  for (int i = 0; i < RW_STEPS; ++i) {
    for (int j = 0; j < N_BLOCKS; ++j) {
      r_continuous.Measure();
      r_continuous.EvalBlock();
    }
    double val = sqrt(r_continuous.GetPrgAve()[0]);
    double err = 0.5 * r_continuous.GetPrgErr()[0] / val;
    fout << (i + 1) << "," << val << "," << err << endl;
  }

  fout.close();

  return 0;
}
