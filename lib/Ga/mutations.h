#ifndef __Mutations__
#define __Mutations__

#include "individual.h"
#include <algorithm>

struct permuteElements : public Mutation {
  permuteElements(double p, size_t i, size_t j) : Mutation(p), i_(i), j_(j){};

  void operator()(Individual &org) { swap(org[i_], org[j_]); };

  size_t i_, j_;
};

struct permuteIntervals : public Mutation {
  permuteIntervals(double p, size_t b1, size_t b2, size_t l)
      : Mutation(p), beg1_(b1), beg2_(b2), len_(l){};

  void operator()(Individual &org) {
    swap_ranges(org.Begin() + beg1_, org.Begin() + beg1_ + len_,
                org.Begin() + beg2_);
  };

  size_t beg1_, beg2_, len_;
};

struct cycleInterval : public Mutation {
  cycleInterval(double p, size_t b, size_t e, size_t s)
      : Mutation(p), beg_(b), end_(e), steps_(s){};

  void operator()(Individual &org) {
    int middle = steps_ % (end_ - beg_) + beg_;
    rotate(org.Begin() + beg_, org.Begin() + middle, org.Begin() + end_);
  };

  size_t beg_, end_, steps_;
};

struct mirrorInterval : public Mutation {
  mirrorInterval(double p, size_t b, size_t e)
      : Mutation(p), beg_(b), end_(e){};

  void operator()(Individual &org) {
    reverse(org.Begin(), org.Begin() + end_);
  };

  size_t beg_, end_;
};

#endif
