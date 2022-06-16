#ifndef __Metropolis__
#define __Metropolis__

#include "../../Point/include/point.h"
#include "../../Random/include/random.h"
#include <functional>
#include <iostream>

using namespace std;

template <size_t DIM, char STEP_TYPE = 'u'> class mrt2 {
public:
  mrt2(double delta) : delta_(delta){};
  bool operator()(point<double, DIM> &p, Random &rnd);
  void Equilibrate(array<double, DIM> &p, size_t steps_); //TODO
  bool IsTuned(double tollerance) { return tollerance_ <= tollerance; };

private:
  double delta_;
  double tollerance_;
  function<double(point<double, DIM>&, point<double, DIM>&)> pdf_ratio_;
};

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

template <size_t DIM, char STEP_TYPE>
bool mrt2<DIM, STEP_TYPE>::operator()(point<double, DIM> &p, Random &rnd) {
  auto m = [&rnd, delta = this->delta_](point<double, DIM> &p) {
    for (double &x : p.ToArray())
      x += TrialStep<STEP_TYPE>(rnd, delta);
    return 1;
  };
  point<double, DIM> q(p.ToArray(), m);
  q.Move();
  double test = pdf_ratio_(q, p);
  if (test >= 1.) {
    p.ToArray() = q.ToArray();
    return 1;
  } else if (rnd.Rannyu() < test) {
    p.ToArray() = q.ToArray();
    return 1;
  }

  return 0;
}

#endif
