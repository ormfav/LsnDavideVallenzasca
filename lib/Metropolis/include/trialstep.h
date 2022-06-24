#ifndef __TrialStep__
#define __TrialStep__

#include "../../Random/include/random.h"
#include <iostream>

using namespace std;

template <char STEP_TYPE>
double TrialStep(double x, Random &rnd, double delta) {
  cout << "ERROR: unrecognized step type. Available options:\n";
  cout << "\'u\' = uniform step\n";
  cout << "\'g\' = gaussian step\n";
  exit(1);
}

template <> double TrialStep<'u'>(double x, Random &rnd, double delta) {
  return x + rnd.Rannyu(-delta, delta);
}

template <> double TrialStep<'+'>(double x, Random &rnd, double delta) {
  double r;
  do {
    r = x + rnd.Rannyu(-delta, delta);
  } while (r == 0);
  int sign = (r > 0) - (r < 0);
  return sign * r;
}

template <> double TrialStep<'g'>(double x, Random &rnd, double delta) {
  return rnd.Gauss(x, delta);
}

#endif
