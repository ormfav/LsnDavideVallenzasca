#ifndef __Generation__
#define __Generation__

#include "../Random/random.h"
#include "chromosome.h"
#include <algorithm>
#include <cmath>
#include <tuple>

template <typename T> class Generation {
public:
  Generation(){};

  size_t Select(Random &, double selection, int elite);

  void ApplyMutation(Chromosome &, Mutation);
  void Evolve(Random &, double selection, int elite, double p_crossover,
              double p_permelm, double p_permint, double p_cycle,
              double p_mirror);
  double BestCost(double);

private:
  vector<Chromosome> population_;
  Decoder<T> *dec;
};

template <typename T>
size_t Generation<T>::Select(Random &rnd, double selection, int elite) {
  size_t i = (population_.size() - elite) * pow(rnd.Rannyu(), selection);
  return i + elite;
}

// mancano probabilità
template <typename T>
void Generation<T>::Evolve(Random &rnd, double selection, int elite,
                           double p_crossover, double p_permelm,
                           double p_permint, double p_cycle, double p_mirror) {

  size_t ip1 = Select(rnd, selection, elite);
  size_t ip2;
  do {
    ip2 = Select(rnd, ip1);
  } while (ip2 == ip1);

  vector<Chromosome> offspring(population_.size());

  for (size_t i = 0; i < offspring.size(); i += 2) {
    /* Crossover */
    Crossover crv(p_crossover);
    if (rnd.Rannyu() < crv.p_)
      tie(offspring[i], offspring[i + 1]) =
          crv(population_[ip1], population_[ip2]);
    else {
      offspring[i] = population_[ip1];
      offspring[i + 1] = population_[ip2];
    }

    /* Mutations */
    for (size_t j = 0; j < 2; ++j) {
      size_t m, n, l;
      Chromosome *pt_son = &offspring[i + j];

      m = rnd.Rannyu(1, pt_son->Size());
      do {
        n = rnd.Rannyu() * pt_son->Size();
      } while (m == n);
      ApplyMutation(pt_son, PermuteElements(p_permelm, m, n));

      m = rnd.Rannyu(1, 0.5 * pt_son->Size());
      n = rnd.Rannyu(0.5 * pt_son->Size(), pt_son->Size());
      l = rnd.Rannyu(0, n - m);
      ApplyMutation(pt_son, PermuteIntervals(p_permint, m, n, l));

      m = rnd.Rannyu(1, pt_son->Size() - 2);
      n = rnd.Rannyu(m, pt_son->Size());
      l = rnd.Rannyu(1, pt_son->Size());
      ApplyMutation(pt_son, CycleInterval(p_cycle, m, n, l));

      m = rnd.Rannyu(1, pt_son->Size() - 2);
      n = rnd.Rannyu(m, pt_son->Size());
      ApplyMutation(pt_son, MirrorInterval(p_mirror, m, n));
    }

    offspring[i].EvalCost(dec);
    offspring[i + 1].EvalCost(dec);
  }

  population_ = offspring;
  sort(population_.begin(), population_.end());
}

#endif
