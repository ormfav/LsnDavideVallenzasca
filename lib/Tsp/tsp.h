#ifndef __Tsp__
#define __Tsp__

#include "../Random/random.h"
#include <cstddef>
#include <iterator>
#include <vector>

using namespace std;

template <typename T> class Chromosome{
public:
  Chromosome(){};
  void EvalCost(vector<T>&);

  int &operator[](int i) { return genes_[i]; };
  bool operator>(Chromosome &c) { return cost_ > c.cost; };
  bool operator<(Chromosome &c) { return cost_ < c.cost; };
  bool operator<=(Chromosome &c) { return !(*this > c); };
  bool operator>=(Chromosome &c) { return !(*this < c); };

  size_t Size(){ return genes_.size();};

  double p_permvalues;
  void MutPermValues(int i, int j);
  double p_permintervals;
  void MutPermIntervals(int begin1, int begin2, int len);
  double p_cycle;
  void MutCycle(int begin, int end, int steps);
  double p_mirror;
  void MutMirror(int begin, int end);

  void Check();

private:
  vector<int> genes_;
  double cost_;
};



/******************/

template <typename T> class Generation{
public:
  Generation(){};

  size_t Select(Random&);
  size_t Select(Random&, size_t);

  void Evolve(Random&);
  double BestCost(double);


private:
  vector<Chromosome<T>> population_;
  vector<T> *dict_;
  int igen_;
  double crossover_;
  double selection_;
  /* int elite; */
};



#endif
