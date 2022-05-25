#ifndef __Tsp__
#define __Tsp__

#include "../../lib/Random/random.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

/* Members are x^2+y^2 and -2xy: when evaluating the distance from city1 */
/* to city2 we do
 * (x1-x2)^2+(y1-y2)^2=(x1^2+y1^2)+(-2x1y1)+(x2^2+y2^2)+(-2x2y2)*/
/* Since we are doing this many times it's easyer to carry out those operations
 * when */
/* creating the object */
struct City {
  /* Methods */
  City(double, double);

  double operator-(City c) { return a + b + c.a + c.b; }; // distance^2

  array<double, 2> GetCoords();

  /* Members */
  double a;
  double b;
};

/***************************************/

struct Chromosome {
  /* Methods */
  Chromosome(){};
  Chromosome(vector<City>, Random &);

  int &operator[](int i) { return this->path[i]; };
  bool operator>(Chromosome &c) { return this->cost > c.cost; };
  bool operator<(Chromosome &c) { return this->cost < c.cost; };
  bool operator<=(Chromosome &c) { return !(*this > c); };
  bool operator>=(Chromosome &c) { return !(*this < c); };

  int Size(){return path.size();};

  void EvalCost(vector<City>);

  void MutationPermute(int, int);
  void MutationPermuteInterval(int start1, int start2, int lenght);
  void MutationCycle(int start, int end, int steps);
  void MutationMirror(int start, int end);

  /* Members */
  vector<int> path;
  double cost;
};

/***************************************/

// set elite
struct Population {

  /* Methods */
  Population(vector<City>, int);
  int Select(Random &);
  int Select(Random &, int);
  double BestCost(double frac); //Best cost averaged over the (frac*100)% best elements - at least 1

  void Sort();

  void NextGeneration(vector<City>, Random&);


  /* Members */
  vector<Chromosome> p;
  int igen;

  double selection_factor;
  // Number of elites
  double elite;
  // Probabilites
  double mutation;
  double crossover;
};

#endif // !__Tsp__
