#include "chromosome.h"
#include <numeric>

Chromosome::Chromosome(size_t N, Random &rnd) : genes_(N) {
  iota(genes_.begin(), genes_.end(), 0);
  for (int i = 1; i < N; ++i) {
    swap(genes_[i], genes_[(int)rnd.Rannyu(1, genes_.size())]);
  }
}

/*******************/
/* Unary operators */
/*******************/


/********************/
/* Binary operators */
/********************/
