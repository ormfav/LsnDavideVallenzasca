#include "tsp.h"
#include <cmath>

template <typename T> size_t Generation<T>::Select(Random &rnd) {
  return 1 + population_.size() * pow(rnd.Rannyu(), selection_);
}

template <typename T> size_t Generation<T>::Select(Random &rnd, size_t prev_i) {
  size_t i;
  do {
    i = population_.size() * pow(rnd.Rannyu(), selection_) + 1;
  } while (i == prev_i);
  return i;
}

template <typename T> void Generation<T>::Evolve(Random &rnd) {
  size_t ip1 = Select(rnd);
  size_t ip2 = Select(rnd, ip1);
  size_t size = population_.Size();
  vector<Chromosome<T>> offspring;
  offspring.reserve(size);

  for (size_t i = 0; i < size; ++i) {
    if (rnd.Rannyu() < crossover_)
      Crossover(population_[ip1], population_[ip2], offspring[i],
                offspring[++i]);
    else {
      offspring[i] = population_[ip1];
      offspring[++i] = population_[ip2];
    }
  }
  double r = rnd.Rannyu();
}
