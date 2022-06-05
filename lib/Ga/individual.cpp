#include "individual.h"
#include <algorithm>
#include <numeric>
#include <set>

Individual::Individual(size_t N, Random &rnd) : chromosome_(N) {
  iota(chromosome_.begin(), chromosome_.end(), 0);
  for (int i = 1; i < N; ++i) {
    swap(chromosome_[i], chromosome_[(int)rnd.Rannyu(1, chromosome_.size())]);
  }
}

bool Individual::Check(){
  set<int> no_duplicates(chromosome_.begin(), chromosome_.end());
  return no_duplicates.size()!=chromosome_.size();
}

