#ifndef __Chromosome__
#define __Chromosome__

#include "../Random/random.h"
#include "decoder.h"
#include <functional>
#include <numeric>
#include <vector>

using namespace std;

class Chromosome {
public:
  Chromosome(size_t, Random &);
  template <typename T> Chromosome(Decoder<T> *, Random &);

  template <typename T> void EvalCost(Decoder<T> *);

  int &operator[](int i) { return genes_[i]; };
  size_t Size() { return genes_.size(); };

  bool operator>(Chromosome &c) { return cost_ > c.cost_; };
  bool operator<(Chromosome &c) { return cost_ < c.cost_; };
  bool operator<=(Chromosome &c) { return !(*this > c); };
  bool operator>=(Chromosome &c) { return !(*this < c); };

  void Check();

private:
  vector<int> genes_;
  double cost_;
};

template <typename T>
Chromosome::Chromosome(Decoder<T> *dec, Random &rnd)
    : Chromosome(dec->dict.size(), rnd) {
  EvalCost(dec);
}

template <typename T> void Chromosome::EvalCost(Decoder<T> *dec) {
  vector<T> ord_dict;
  ord_dict.reserve(dec->dict.size());
  for (size_t i = 0; i < dec->dict.size(); ++i)
    ord_dict.push_back(dec->dict[genes_[i]]);
  cost_ = dec.costf(ord_dict);
};

/*******************/
/* Unary operators */
/*******************/
struct Mutation {
  Mutation(double p) : p_(p){};
  virtual void operator()(Chromosome &);
  double p_;
};

struct PermuteElements : public Mutation {
  PermuteElements(double p, size_t i, size_t j) 
      : Mutation(p), i_(i), j_(j){};
  void operator()(Chromosome &);
  size_t i_, j_;
};

struct PermuteIntervals : public Mutation {
  PermuteIntervals(double p, size_t b1, size_t b2, size_t l)
      : Mutation(p), beg1_(b1), beg2_(b2), len(l){};
  void operator()(Chromosome &);
  size_t beg1_, beg2_, len;
};

struct CycleInterval : public Mutation {
  CycleInterval(double p, size_t b, size_t e, size_t s)
      : Mutation(p), beg_(b), end_(e), steps_(s){};
  void operator()(Chromosome &);
  size_t beg_, end_, steps_;
};

struct MirrorInterval : public Mutation {
  MirrorInterval(double p, size_t b, size_t e)
      : Mutation(p), beg_(b), end_(e){};
  void operator()(Chromosome &);
  size_t beg_, end_;
};


/********************/
/* Binary operators */
/********************/
struct Crossover {
  Crossover(double p) : p_(p){};
  pair<Chromosome, Chromosome> operator()(Chromosome &, Chromosome &);
  double p_;
};

#endif
