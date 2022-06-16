#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>

using namespace std;
using pt3D = point<double, 3>;

#include "../in/022-conf.inl"
#include "../include/022.h"

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* functions */
  queue<array<double, 3>> store_rw_pos;
  auto db_move =
      [&rnd, &store_rw_pos](pt3D &p,
                            function<void(pt3D &, Random &, double)> rw_move) {
        p.Jump(store_rw_pos.front());
        store_rw_pos.pop();
        rw_move(p, rnd, LATTICE_DIM);
        store_rw_pos.push(p.ToArray());
        return 1;
      };
  array<function<double(pt3D)>, 1> db_eval = {
      [](pt3D p) { return p.Lenght2(); }};

  /* -------- */
  /* Discrete  */
  /* -------- */

  /* Initializing data blocking */
  dataBlocks<3, 1> r_discrete(
      RW_PER_BLOCK, bind(db_move, placeholders::_1, RwDiscrete), db_eval);
  for (size_t i = 0; i < RW_PER_BLOCK * N_BLOCKS; ++i)
    store_rw_pos.push({0});

  /* Initializing output file */
  string path = "out/022-discreterw.csv";
  ofstream fout(path);
  FileCheck(fout, path);
  fout << "step,r_ave,r_err\n";

  /* Data blocking  */
  for (size_t i = 0; i < RW_STEPS; ++i) {
    r_discrete.Restart();
    do {
      r_discrete.Measure();
      r_discrete.EvalBlock();
    } while (r_discrete.CompletedBlocks() < N_BLOCKS);
    double val = sqrt(r_discrete.GetPrgAve()[0]);
    double err = 0.5 * r_discrete.GetPrgErr()[0] / val;
    fout << (i + 1) << "," << val << "," << err << endl;
  }

  fout.close();

  /* ---------- */
  /* Continuous */
  /* ---------- */

  /* Initializing data blocking */
  dataBlocks<3, 1> r_continuous(
      RW_PER_BLOCK, bind(db_move, placeholders::_1, RwContinuous), db_eval);
  for (size_t i = 0; i < RW_PER_BLOCK * N_BLOCKS; ++i) {
    store_rw_pos.pop();
    store_rw_pos.push({0});
  }

  /* Initializing output file */
  path = "out/022-continuousrw.csv";
  fout.open(path);
  FileCheck(fout, path);
  fout << "step,r_ave,r_err\n";

  /* Data blocking  */
  for (size_t i = 0; i < RW_STEPS; ++i) {
    r_continuous.Restart();
    do {
      r_continuous.Measure();
      r_continuous.EvalBlock();
    } while (r_continuous.CompletedBlocks() < N_BLOCKS);
    double val = sqrt(r_continuous.GetPrgAve()[0]);
    double err = 0.5 * r_continuous.GetPrgErr()[0] / val;
    fout << (i + 1) << "," << val << "," << err << endl;
  }

  fout.close();

  return 0;
}
