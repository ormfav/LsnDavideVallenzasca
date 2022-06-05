#ifndef __Crossovers_
#define __Crossovers_
#include "individual.h"

struct fixHead : public Crossover {
  fixHead(double p, size_t cut_pos) : Crossover(p), cut_pos_(cut_pos){};
  pair<Individual, Individual> operator()(Individual &, Individual &);

  size_t cut_pos_;
};

pair<Individual, Individual> fixHead::operator()(Individual &parent1,
                                                 Individual &parent2) {
  Individual son1 = parent1;
  Individual son2 = parent2;

  for (int ip2 = 0, is1 = cut_pos_; is1 < son1.Size(); ++ip2) {
    for (int ip1 = cut_pos_; ip1 < parent1.Size(); ++ip1) {
      if (parent2[ip2] == parent1[ip1]) {
        son1[is1++] = parent1[ip1];
        break;
      }
    }
  }

  for (int ip1 = 0, is2 = cut_pos_; is2 < son2.Size(); ++ip1) {
    for (int ip2 = cut_pos_; ip2 < parent2.Size(); ++ip2) {
      if (parent1[ip1] == parent2[ip2]) {
        son2[is2++] = parent2[ip2];
        break;
      }
    }
  }
  
  return {son1, son2};
}

#endif

