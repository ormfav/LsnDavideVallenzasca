#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Metropolis/include/metropolis.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"

using pt1D = point<double, 1>;

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
}

pair<double, double> EvalEnergy(array<double, 2> &params, double delta,
                                size_t steps_per_block, size_t n_blocks,
                                Random &rnd) {
  /* Initializing metropolis */
  auto pdf_ratio = [&params](pt1D q, pt1D p) {
    return Psi2_gs(q, params) / Psi2_gs(p, params);
  };

  mrt2<1> metro(delta, pdf_ratio, rnd);
  array<double, 1> p0 = {0};
  metro.AutoTune(p0, 0.05, 500);

  /* Initializing data blocking */
  auto db_eval = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> energy(steps_per_block, metro, {db_eval}, p0);

  /* Data blocking */
  do {
    energy.Measure();
    energy.EvalBlock();
  } while (energy.CompletedBlocks() < n_blocks);

  return {energy.GetPrgAve()[0], energy.GetPrgErr()[0]};
}

void EvalEnergy(array<double, 2> &params, ofstream &fout_pt,
                array<ofstream, 1> &fout_ene, double delta,
                size_t steps_per_block, size_t n_blocks, Random &rnd) {
  /* Initializing metropolis */
  auto pdf_ratio = [&params](pt1D q, pt1D p) {
    return Psi2_gs(q, params) / Psi2_gs(p, params);
  };

  mrt2<1> metro(delta, pdf_ratio, rnd);
  array<double, 1> p0 = {0};
  metro.AutoTune(p0, 0.05, 500);

  /* Initializing data blocking */
  auto db_eval = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> energy(steps_per_block, metro, {db_eval}, p0);

  /* Data blocking */

  do {
    energy.Measure(10, fout_pt);
    energy.EvalBlock(fout_ene);
  } while (energy.CompletedBlocks() < n_blocks);
}
