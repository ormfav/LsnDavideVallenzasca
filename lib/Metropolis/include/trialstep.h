#ifndef __TrialStep__
#define __TrialStep__

#include "../../Random/include/random.h"
#include <iostream>

using namespace std;

template <char STEP_TYPE> double TrialStep(Random &rnd, double delta) {
  cout << "ERROR: unrecognized step type. Available options:\n";
  cout << "\'u\' = uniform step\n";
  cout << "\'g\' = gaussian step\n";
  exit(1);
}
template <> double TrialStep<'u'>(Random &rnd, double delta) {
  return rnd.Rannyu(-delta, delta);
}
template <> double TrialStep<'g'>(Random &rnd, double delta) {
  return rnd.Gauss(0, delta);
}


#endif
