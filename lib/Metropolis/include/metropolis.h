#ifndef __Metropolis__
#define __Metropolis__

#include "../../Point/include/point.h"
#include "trialstep.h"
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;

template <size_t DIM, char STEP_TYPE = 'u'> class mrt2 {
public:
  mrt2(double delta,
       function<double(point<double, DIM> &, point<double, DIM> &)> pdf_ratio,
       Random &);

  bool operator()(point<double, DIM> &p);

  double Equilibrate(array<double, DIM> &p, size_t steps);
  double Equilibrate(array<double, DIM> &p, size_t steps,
                     function<double(point<double, DIM>)>, ofstream &);
  double Equilibrate(array<double, DIM> &p, size_t steps, ofstream &);
  bool IsTuned(double tollerance) { return tollerance_ <= tollerance; };
  void AutoTune(array<double, DIM> &p, double tollerance, size_t steps);

private:
  double delta_;
  function<double(point<double, DIM> &, point<double, DIM> &)> pdf_ratio_;
  double tollerance_;

  Random *rnd_;
};

template <size_t DIM, char STEP_TYPE>
mrt2<DIM, STEP_TYPE>::mrt2(
    double delta,
    function<double(point<double, DIM> &, point<double, DIM> &)> pdf_ratio,
    Random &rnd)
    : delta_(delta), pdf_ratio_(pdf_ratio), tollerance_(0), rnd_(&rnd) {}

template <size_t DIM, char STEP_TYPE>
bool mrt2<DIM, STEP_TYPE>::operator()(point<double, DIM> &p) {

  auto m = [this](point<double, DIM> &p) {
    for (double &x : p.ToArray())
      x = TrialStep<STEP_TYPE>(x, *(this->rnd_), this->delta_);
    return 1;
  };
  point<double, DIM> q(m, p.ToArray());
  q.Move();
  double test = pdf_ratio_(q, p);
  if (test >= 1.) {
    p.ToArray() = q.ToArray();
    return 1;
  } else if (rnd_->Rannyu() < test) {
    p.ToArray() = q.ToArray();
    return 1;
  }

  return 0;
}

template <size_t DIM, char STEP_TYPE>
double mrt2<DIM, STEP_TYPE>::Equilibrate(array<double, DIM> &p, size_t steps) {
  point<double, DIM> q(*this, p);

  for (size_t i = 0; i < steps; ++i)
    q.Move();

  p = q.ToArray();
  return q.Rate();
}

template <size_t DIM, char STEP_TYPE>
double mrt2<DIM, STEP_TYPE>::Equilibrate(array<double, DIM> &p, size_t steps,
                                         function<double(point<double, DIM>)> f,
                                         ofstream &fout) {
  point<double, DIM> q(*this, p);

  for (size_t i = 0; i < steps; ++i) {
    fout << i << "," << f(q) << endl;
    q.Move();
  }

  p = q.ToArray();
  return q.Rate();
}

template <size_t DIM, char STEP_TYPE>
double mrt2<DIM, STEP_TYPE>::Equilibrate(array<double, DIM> &p, size_t steps,
                                         ofstream &fout) {
  point<double, DIM> q(*this, p);

  for (size_t i = 0; i < steps; ++i) {
    string to_print = "";
    q.Move();
    for (double x : q.ToArray()) {
      to_print.append(to_string(x));
      to_print.push_back(',');
    }
    to_print.pop_back(); //delete last comma
    fout << to_print << endl;
  }

  p = q.ToArray();
  return q.Rate();
}

template <size_t DIM, char STEP_TYPE>
void mrt2<DIM, STEP_TYPE>::AutoTune(array<double, DIM> &p, double tollerance,
                                    size_t steps) {

  point<double, DIM> r(*this, p);
  double dmin = 0.;
  double dmax = 0.;
  double tollmin = 0.5 - tollerance;
  double tollmax = 0.5 + tollerance;
  double acc;
  auto display_info = [&]() {
    cout << "Interval of tollerance: [" << tollmin << "," << tollmax << "]\n";
    cout << "Current acceptance rate: " << acc << endl;
    cout << "----------\n";
  };

  /* First part: finding an interval */
  do {
    point<double, DIM> q(*this, p);
    cout << dmin << " " << delta_ << " " << dmax << endl;
    for (size_t i = 0; i < steps; ++i)
      q.Move();
    r = q;
    acc = q.Rate();
    display_info();
    if (acc < tollmin) {
      dmax = delta_;
      delta_ *= 0.8;
    } else if (acc > tollmax) {
      dmin = delta_;
      delta_ *= 1.2;
    } else {
      tollerance_ = fabs(0.5 - acc);
      p = q.ToArray();
      return;
    }
  } while (dmin * dmax == 0);

  /* Second part: bisection method */
  do {
    point<double, DIM> q(*this, p);
    delta_ = 0.5 * (dmax + dmin);
    cout << dmin << " " << delta_ << " " << dmax << endl;
    for (size_t i = 0; i < steps; ++i)
      q.Move();
    r = q;
    acc = q.Rate();
    tollerance_ = fabs(acc - 0.5);
    display_info();
    if (acc < 0.5)
      dmax = delta_;
    else
      dmin = delta_;
  } while (!IsTuned(tollerance));

  p = r.ToArray();
}

#endif
