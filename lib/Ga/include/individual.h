#ifndef __Individual__
#define __Individual__

#include "../../Random/include/random.h"
#include "decoder.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

class Individual {
public:
  Individual(){};
  Individual(size_t, Random &);
  template <typename T> Individual(Decoder<T> *, Random &);

  template <typename T> void EvalCost(Decoder<T> *);
  /* Return a vector ordered according to chromosome_ */
  template <typename T> vector<T> ApplyTo(Decoder<T> *);

  double& GetCost() { return cost_; };

  bool Check(); // For debugging pourpose, can be used in assert

  size_t &operator[](size_t i) { return chromosome_[i]; };
  size_t Size() { return chromosome_.size(); };
  vector<size_t>::iterator Begin() { return chromosome_.begin(); };
  vector<size_t>::iterator End() { return chromosome_.end(); };

  bool operator>(Individual &c) { return cost_ > c.cost_; };
  bool operator<(Individual &c) { return cost_ < c.cost_; };
  bool operator<=(Individual &c) { return !(*this > c); };
  bool operator>=(Individual &c) { return !(*this < c); };

private:
  vector<size_t> chromosome_;
  double cost_;
};

template <typename T>
Individual::Individual(Decoder<T> *dec, Random &rnd)
    : Individual(dec->dict_.size(), rnd) {
  EvalCost(dec);
}

template <typename T> void Individual::EvalCost(Decoder<T> *dec) {
  cost_ = dec->costf_(ApplyTo(dec));
};

template <typename T> vector<T> Individual::ApplyTo(Decoder<T> *dec) {
  vector<T> ordered_dict;
  ordered_dict.reserve(dec->dict_.size());

  auto fill = [&](size_t i) { ordered_dict.push_back(dec->dict_[i]); };
  for_each(chromosome_.begin(), chromosome_.end(), fill);

  return ordered_dict;
};

/*****************************/
/* Mutation - unary operator */
/*****************************/

struct Mutation {
  Mutation(double p) : p_(p){};
  virtual void operator()(Individual &) = 0;
  double p_;
};

/*******************************/
/* Crossover - Binary operator */
/*******************************/

struct Crossover {
  Crossover(double p) : p_(p){};
  virtual pair<Individual, Individual> operator()(Individual &,
                                                  Individual &) = 0;
  double p_;
};

#endif
