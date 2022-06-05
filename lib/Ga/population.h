#ifndef __Generation__
#define __Generation__

#include "../Random/random.h"
#include "individual.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <tuple>

template <typename T> class Population {
public:
  Population(size_t, Decoder<T> &, Random &);

  void SetElite() { elite_ = 1; };
  void UnsetElite() { elite_ = 0; };

  size_t Select(Random &, double selection);

  void Evolve(Random &, double selection, Crossover, vector<Mutation>);
  double BestCost(double);

private:
  vector<Individual> population_;
  Decoder<T> *dec_;
  bool elite_;
};

template <typename T>
Population<T>::Population(size_t n_individuals, Decoder<T> &dec, Random &rnd)
    : elite_(0) {
  dec_ = &dec;
  population_.reserve(n_individuals);
  for (size_t i = 0; i < n_individuals; ++i)
    population_.emplace_back(dec_, rnd);
}

template <typename T>
size_t Population<T>::Select(Random &rnd, double selection) {
  if (elite_)
    return 1 + (population_.size() - 1) * pow(rnd.Rannyu(), selection);
  else
    return population_.size() * pow(rnd.Rannyu(), selection);
}

template <typename T>
void Population<T>::Evolve(Random &rnd, double selection, Crossover crs,
                           vector<Mutation> mut) {
  vector<Individual> offspring(population_.size());
  size_t ip1 = Select(rnd, selection);
  size_t ip2;
  do {
    ip2 = Select(rnd, selection);
  } while (ip2 == ip1);

  for (size_t i = 0; i < offspring.size(); i += 2) {
    /* Crossover */
    if (rnd.Rannyu() < crs.p_)
      tie(offspring[i], offspring[i + 1]) =
          crs(population_[ip1], population_[ip2]);
    else {
      offspring[i] = population_[ip1];
      offspring[i + 1] = population_[ip2];
    }

    /* Mutations */
    for (size_t j = 0; j < 2; ++j) {
      for (auto &x : mut) {
        if (rnd.Rannyu() < x.p_)
          x(offspring[i + j]);
      }
    }

    offspring[i].EvalCost(dec_);
    offspring[i + 1].EvalCost(dec_);
  }

  population_ = offspring;
  sort(population_.begin(), population_.end());
}

template <typename T> double Population<T>::BestCost(double frac) {
  size_t imax = population_.size() * frac;
  ++imax;
  double ave = 0;
  for (int i = 0; i < imax; ++i)
    ave += population_[i].GetCost();
  return ave / imax;
};

#endif
