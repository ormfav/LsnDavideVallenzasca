#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Metropolis/include/metropolis.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"

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

#include "../in/082-conf_db.inl"
#include "../in/082-conf_metro.inl"
array<double, 2> EvalEnergy(array<double, 2> &params, Random &rnd) {
  /* Initializing metropolis */
  auto pdf_ratio = [&params](point<double, 1> x, point<double, 1> y) {
    return Psi2_gs(x, params) / Psi2_gs(x, params);
  };

  mrt2<1> metro(DELTA, pdf_ratio, rnd);

  array<double, 1> p0 = {0};
  metro.AutoTune(p0, 0.1, 1000);

  /* Initializing data blocking */
  auto db_eval = bind(Integrand, placeholders::_1, params);
  dataBlocks<1, 1> energy(STEPS_PER_BLOCK, metro, {db_eval}, p0);

  /* Data blocking */
  do {
    energy.Measure();
    energy.EvalBlock();
  } while (energy.CompletedBlocks() < N_BLOCKS);

  return {energy.GetPrgAve()[0], energy.GetPrgErr()[0]};
}

struct boltzmanRatio {
  boltzmanRatio(double beta, Random &rnd, array<double, 2> &q);
  double operator()(point<double, 2> &);
  void UpdateEnergy() { prev_ene_ = trial_ene_; };
  void UpdateBeta() { beta_ += 1.5; };

  double beta_;
  /* energy+err of the previous step */
  array<double, 2> prev_ene_;
  /* store energy+err waiting for it to be accepted */
  array<double, 2> trial_ene_;
  Random *rnd_;
};

boltzmanRatio::boltzmanRatio(double beta, Random &rnd, array<double, 2> &q)
    : beta_(beta), rnd_(&rnd) {
  prev_ene_ = EvalEnergy(q, *rnd_);
};

double boltzmanRatio::operator()(point<double, 2> &q) {
  trial_ene_ = EvalEnergy(q.ToArray(), *rnd_);

  double dH = prev_ene_[0] - trial_ene_[0];
  return exp(beta_ * dH);
}
