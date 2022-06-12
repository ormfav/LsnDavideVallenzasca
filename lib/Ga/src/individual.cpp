#include "../include/individual.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <set>

Individual::Individual(size_t N, Random &rnd) : chromosome_(N) {
  iota(chromosome_.begin(), chromosome_.end(), 0);
  // posso miglioreare swap??
  for (size_t i = 1; i < N; ++i)
    swap(chromosome_[i], chromosome_[(int)rnd.Rannyu(1, N)]);
}

bool Individual::Check() {
  set<int> no_duplicates(chromosome_.begin(), chromosome_.end());
  return no_duplicates.size() != chromosome_.size();
}
