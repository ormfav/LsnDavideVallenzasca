#include "../../lib/Point/include/point.h"

double Integrand(point<double,1> p, array<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double x = p[0];
  double x2 = x * x;

  double kin = mu * mu + x2 - 2 * mu * x * tanh(mu * x / sigma2) - sigma2;
  double pot = x2 * x2 - 2.5 * x2;

  return pot - 0.5 * kin / (sigma2 * sigma2);
}

double Psi2_gs(point<double,1> p, array<double, 2> &params) {
  double mu = params[0], sigma2 = params[1];
  double psi = exp(-0.5 * (p[0] - mu) * (p[0] - mu) / sigma2) +
               exp(-0.5 * (p[0] + mu) * (p[0] + mu) / sigma2);
  return psi * psi;
};

