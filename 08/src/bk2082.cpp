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
using pt2D = point<double, 2>;

#include "../include/082.h"

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* SA */
  array<double, 2> starting_params = {1., 1.};
  boltzmanRatio boltzmanratio(1, rnd, starting_params);
  auto pdf_ratio = [&boltzmanratio](pt2D &q, pt2D &p) {
    return boltzmanratio(q);
  };
  mrt2<2> metro(1, pdf_ratio, rnd);
  auto move = [&metro, &boltzmanratio](pt2D &p) {
    if (metro(p)) {
      boltzmanratio.UpdateEnergy();
      return 1;
    }
    return 0;
  };

  /* Preparing output files */
  array<ofstream, 2> fout_sa;
  array<string, 2> path_sa = {"out/082-sa_energy.csv", "out/082-sa_params.csv"};
  for (size_t i = 0; i < 2; ++i) {
    fout_sa[i].open(path_sa[i]);
    FileCheck(fout_sa[i], path_sa[i]);
  }
  fout_sa[0] << "sa_step,energy,err\n";
  fout_sa[1] << "sa_step,mu,sigma^2\n";

  pt2D params(move, starting_params);
  for (size_t i = 0; i < 50; ++i) {
    // C'È BISOGNO DI EQUILIBRAZIONE QUI??
    /* metro.AutoTune(p0, 0.1, 1000); */
    for (size_t n = 0; n < 40; ++n) {
      params.Move();
      fout_sa[0] << (i * 40 + n) << boltzmanratio.prev_ene_[0] << ","
                 << boltzmanratio.prev_ene_[1] << endl;
      fout_sa[1] << (i * 40 + n) << params[0] << "," << params[1] << endl;
    }
    boltzmanratio.UpdateBeta();
  }

  /* Once we get the best parameters... */
  /* Initializing metropolis */
  auto db_pdf_ratio = [&params](point<double, 1> x, point<double, 1> y) {
    return Psi2_gs(x, params.ToArray()) / Psi2_gs(x, params.ToArray());
  };

  mrt2<1> db_metro(DELTA, db_pdf_ratio, rnd);

  array<double, 1> p0 = {0};
  db_metro.AutoTune(p0, 0.1, 1000);

  /* Initializing data blocking */
  auto db_eval = bind(Integrand, placeholders::_1, params.ToArray());
  dataBlocks<1, 1> best_energy(STEPS_PER_BLOCK, db_metro, {db_eval}, p0);

  /* Preparing output files */
  string path_db_points = "out/082-db_points.csv";
  ofstream fout_db_points(path_db_points);
  FileCheck(fout_db_points, path_db_points);
  fout_db_points << "sa_step,mu,sigma^2\n";

  string path_db_energy = "out/082-db_energy.csv";
  array<ofstream, 1> fout_db_energy;
  fout_db_energy[0].open(path_db_energy);
  FileCheck(fout_db_energy[0], path_db_energy);
  fout_db_energy[0] << "sa_step,energy,err\n";

  /* Data blocking */
  do {
    best_energy.Measure(1, fout_db_points);
    best_energy.EvalBlock(fout_db_energy);
  } while (best_energy.CompletedBlocks() < N_BLOCKS);

  return 0;
}
