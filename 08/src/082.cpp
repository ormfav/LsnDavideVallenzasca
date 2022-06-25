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

#include "../in/082-conf_db.inl"
#include "../in/082-conf_metro.inl"
#include "../include/082.h"

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  double oldH, olderr;
  /* To avoid going throug energy evaluation two times per metropolis step.
   * tempH stores new energy and if move is accepted copies its value in oldH */
  double tempH, temperr;

  double beta = 1;

  auto boltzmannratio = [&](pt2D &trial_p, pt2D &p) {
    tie(tempH, temperr) =
        EvalEnergy(trial_p.ToArray(), DELTA_EN, STEPS_PER_BLOCK, N_BLOCKS, rnd);

    array<double, 2> num, den;
    for (size_t i = 0; i < 2; ++i) {
      if (p[i] >= DELTA_SA - trial_p[i])
        num[i] = trial_p[i] / DELTA_SA;
      else
        num[i] = 1 - trial_p[i] / DELTA_SA;
      if (trial_p[i] >= DELTA_SA - p[i])
        den[i] = p[i] / DELTA_SA;
      else
        den[i] = 1 - p[i] / DELTA_SA;
    }
    double trial_step_ratio = num[0] * num[1] / (den[0] * den[1]);

    return trial_step_ratio * exp(beta * (oldH - tempH));
  };

  /* Metropoplis wrapper to allow energy update */
  mrt2<2, '+'> metro(DELTA_SA, boltzmannratio, rnd);
  auto move_params = [&](pt2D &params) {
    if (metro(params)) {
      oldH = tempH;
      olderr = temperr;
      return 1;
    }
    return 0;
  };

  /* SA */
  array<double, 2> starting_params = {1, 1};

  tie(oldH, olderr) =
      EvalEnergy(starting_params, DELTA_EN, STEPS_PER_BLOCK, N_BLOCKS, rnd);
  pt2D params(move_params, starting_params);

  string path = "out/082-params.csv";
  ofstream fout(path);
  FileCheck(fout, path);
  fout << "mu,sigma^2,energy,err\n";
  fout << params[0] << "," << params[1] << "," << oldH << "," << olderr << endl;

  for (size_t i = 0; i < 30; ++i) {
    cout << "=======================\n";
    cout << "| schedule step: " << (i + 1) << "/30\n";
    cout << "| beta: " << beta << "\n";
    cout << "=======================\n";

    /* size_t steps = 30; */
    size_t steps = 10 + size_t(1.33 * i);
    /* size_t steps = 10 + size_t(pow(i,1.5)/3.9); */
    for (size_t j = 0; j < steps; ++j) {
      cout << "\nstep: " << (j + 1) << "/" << steps << endl;
      params.Move();
      fout << params[0] << "," << params[1] << "," << oldH << "," << olderr
           << endl;
    }
    beta *= 1.2;
  }
  fout.close();

  /* Datablocking with best params */
  string path_pt = "out/082-points.dat";
  string path_ene = "out/082-best_energy.csv";

  ofstream fout_pt(path_pt);
  array<ofstream, 1> fout_ene;
  fout_ene[0].open(path_ene);

  FileCheck(fout_pt, path_pt);
  FileCheck(fout_ene[0], path_ene);

  EvalEnergy(params.ToArray(), fout_pt, fout_ene, DELTA_EN, STEPS_PER_BLOCK,
             N_BLOCKS, rnd);

  fout_pt.close();
  fout_ene[0].close();

  return 0;
}
