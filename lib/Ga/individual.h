#ifndef __Individual__
#define __Individual__

#include "../Random/random.h"
#include "decoder.h"
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

class Individual {
public:
  Individual(size_t, Random &);
  template <typename T> Individual(Decoder<T> *, Random &);

  template <typename T> void EvalCost(Decoder<T> *);
  double GetCost(){ return cost_;};

  bool Check();
  
  int &operator[](int i) { return chromosome_[i]; };
  size_t Size() { return chromosome_.size(); };
  vector<int>::iterator Begin() { return chromosome_.begin(); };
  vector<int>::iterator End() { return chromosome_.end(); };

  bool operator>(Individual &c) { return cost_ > c.cost_; };
  bool operator<(Individual &c) { return cost_ < c.cost_; };
  bool operator<=(Individual &c) { return !(*this > c); };
  bool operator>=(Individual &c) { return !(*this < c); };


private:
  vector<int> chromosome_;
  double cost_;
};

template <typename T>
Individual::Individual(Decoder<T> *dec, Random &rnd)
    : Individual(dec->dict.size(), rnd) {
  EvalCost(dec);
}

template <typename T> void Individual::EvalCost(Decoder<T> *dec) {
  vector<T> ord_dict;
  ord_dict.reserve(dec->dict.size());
  for (size_t i = 0; i < dec->dict.size(); ++i)
    ord_dict.push_back(dec->dict[chromosome_[i]]);
  cost_ = dec.costf(ord_dict);
};

/****************************/
/* Mutation - unary operator*/
/****************************/
struct Mutation {
  Mutation(double p) : p_(p){};
  virtual void operator()(Individual &);
  double p_;
};

/********************************/
/* Crossover - Binary operators */
/********************************/

struct Crossover {
  Crossover(double p) : p_(p){};
  virtual pair<Individual, Individual> operator()(Individual &, Individual &);
  double p_;
};

#endif
