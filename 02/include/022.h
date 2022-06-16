#ifndef __022__
#define __022__

#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <cmath>

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

#endif
