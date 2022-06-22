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

#include "../in/081-conf_db.inl"
#include "../in/081-conf_metro.inl"

double Integrand(point<double, 1> p, point<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double x = p[0];
  double x2 = x * x;

  double kin = mu * mu + x2 - 2 * mu * x * tanh(mu * x / sigma2) - sigma2;
  double pot = x2 * x2 - 2.5 * x2;

  return pot - 0.5 * kin / (sigma2 * sigma2);
}

double Psi2_gs(point<double, 1> p, point<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double psi = exp(-0.5 * (p[0] - mu) * (p[0] - mu) / sigma2) +
               exp(-0.5 * (p[0] + mu) * (p[0] + mu) / sigma2);
  return psi * psi;
};

double BoltzmanRatio(point<double, 2> new_params, double &old_H, double beta,
                     Random &rnd) {
  /* Initializing data blocking */
  function<double(point<double, 1>)> pdf =
      bind(Psi2_gs, placeholders::_1, new_params);
  auto m = bind(Mrt2Unif<1>, placeholders::_1, pdf, rnd, DELTA);
  auto f = bind(Integrand, placeholders::_1, new_params);
  dataBlocks<1, 1> new_H(STEPS_PER_BLOCK, {0}, m, {f});

  /* Data blocking */
  for (size_t db = 0; db < 20; db++) {
    new_H.Measure();
    new_H.EvalBlock();
  }
  double dH = old_H - new_H.GetPrgAve()[0];
  old_H = new_H.GetPrgAve()[0];
  return exp(beta * dH);
}

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Stuff for SA */
  /* Variables */
  double beta = 1;
  double old_H, trial_H;

  /* Functions */
  auto UpdateBeta = [&beta](double a) { beta += a; };
  auto MoveParams = [&](point<double, 2> &params) {
    auto MetroTest = bind(BoltzmanRatio, placeholders::_1, trial_H, beta, rnd);
    if (Mrt2UnifOpt<2>(params, MetroTest, rnd, DELTA)) {
      old_H = trial_H;
      return 1;
    }
    trial_H = old_H;
    return 0;
  };

  /* [0] -> mu, [1] -> sigma^2  */
  point<double, 2> params({1., 1.}, MoveParams);

  /* The actual SA */
  string path = "08/out/082-params_trajectory.csv";
  ofstream fout_traj(path);
  FileCheck(fout_traj, path);
  fout_traj << "mu,sigma\n";

  for (size_t i = 0; i < 50; ++i) {
    cout << "beta: " << beta << endl;
    for (size_t n = 0; n < 40; ++n) {
      params.Move();
      fout_traj << params[0] << "," << params[1] << endl;
    }
    UpdateBeta(1.5);
  }

  /* Initializing data blocking */
  function<double(point<double, 1>)> pdf =
      bind(Psi2_gs, placeholders::_1, params);
  auto m = bind(Mrt2Unif<1>, placeholders::_1, pdf, rnd, DELTA);
  auto f = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> minH(STEPS_PER_BLOCK, {0}, m, {f});

  path = "08/out/082-min_energy.csv";
  array<ofstream, 1> fout_minh;
  fout_minh[0].open(path);
  FileCheck(fout_minh[0], path);

  /* Data blocking */
  for (size_t db = 0; db < 20; db++) {
    minH.Measure();
    minH.EvalBlock(fout_minh);
  }

  fout_minh[0].close();

  return 0;
}
