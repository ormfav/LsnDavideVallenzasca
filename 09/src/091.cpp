#include "../../lib/Ga/include/decoder.h"
#include "../../lib/Ga/include/individual.h"
#include "../../lib/Ga/include/population.h"
#include "../include/crossovers.h"
#include "../include/mutations.h"

#include "../include/city.h"

#include "../../lib/Misc/include/misc.h"
#include "../../lib/Random/include/random.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <set>

using namespace std;

#include "../in/091-conf_ga.inl"
#include "../in/091-conf_simulation.inl"

int main(int argc, char *argv[]) {
  Random rnd("in/Primes", "in/seed.in");

  /* Cities on a circumference */
  Decoder<City> dec_circle;
  dec_circle.costf_ = costfunction;
  set<double> angles;
  do {
    angles.emplace(rnd.Rannyu() * 2 * M_PI);
  } while (angles.size() != N_CITIES);
  dec_circle.dict_.reserve(N_CITIES);
  for (double x : angles)
    dec_circle.dict_.emplace_back(cos(x), sin(x));

  /* Cities on a square */
  Decoder<City> dec_square;
  dec_square.costf_ = costfunction;
  set<City> aux;
  do {
    aux.emplace(rnd.Rannyu(), rnd.Rannyu());
  } while (aux.size() != N_CITIES);
  dec_square.dict_.assign(aux.begin(), aux.end());

  Population<City> pop_circle(INDIVIDUALS, dec_circle, rnd);
  Population<City> pop_square(INDIVIDUALS, dec_square, rnd);

  /* Output first path */
  array<ofstream, 2> fout_cities;
  array<string, 2> path_cities = {"out/091-firstpath_circle.csv",
                                  "out/091-firstpath_square.csv"};
  array<vector<City>, 2> cities = {pop_circle.GetBest().ApplyTo(&dec_circle),
                                   pop_square.GetBest().ApplyTo(&dec_square)};

  for (size_t i = 0; i < 2; ++i) {
    ofstream &fout = fout_cities[i];
    string &path = path_cities[i];
    vector<City> &city = cities[i];

    fout.open(path);
    FileCheck(fout, path);
    fout << "x,y\n";
    for (auto x : city)
      fout << x.a << "," << x.b << endl;

    fout.close();
  }

  /* Preparing output costs */
  array<ofstream, 4> fout_bestcost;
  array<string, 4> path_bestcost = {
      "out/091-bestcost_circle.csv", "out/091-besthalfcost_circle.csv",
      "out/091-bestcost_square.csv", "out/091-besthalfcost_square.csv"};
  for (size_t i = 0; i < 4; ++i) {
    fout_bestcost[i].open(path_bestcost[i]);
    FileCheck(fout_bestcost[i], path_bestcost[i]);
    fout_bestcost[i] << "gen,cost\n";
  }

  /* We are performing two separate loops: using lambda to avoid
   * writing the same code twice */
  auto evolving = [&](size_t i) {
    cout << "Generation: " << (i + 1) << "/" << N_GEN << endl;
    /* Output costs */
    fout_bestcost[0] << i << "," << pop_circle.BestCost(0) << endl;
    fout_bestcost[1] << i << "," << pop_circle.BestCost(0.5) << endl;
    fout_bestcost[2] << i << "," << pop_square.BestCost(0) << endl;
    fout_bestcost[3] << i << "," << pop_square.BestCost(0.5) << endl;

    /* Building crossover */
    fixHead crs(P_CROSSOVER, rnd.Rannyu(1, N_CITIES - 1));

    int x, y, z;
    /* Building mutations */
    x = rnd.Rannyu(1, N_CITIES);
    do {
      y = rnd.Rannyu(1, N_CITIES);
    } while (x == y);
    permuteElements m1(P_PERMELM, x, y);

    x = rnd.Rannyu(1, int(N_CITIES * 0.5));
    y = rnd.Rannyu(x, N_CITIES);
    z = rnd.Rannyu(1, min(y - x, N_CITIES - y));
    permuteIntervals m2(P_PERMINT, x, y, z);

    x = rnd.Rannyu(1, N_CITIES - 2);
    y = rnd.Rannyu(x + 1, N_CITIES);
    z = rnd.Rannyu(1, N_CITIES);
    cycleInterval m3(P_CYCLEINT, x, y - x, z);

    x = rnd.Rannyu(1, N_CITIES - 2);
    y = rnd.Rannyu(x, N_CITIES);
    mirrorInterval m4(P_MIRRORINT, x, y - x);

    /* Evolving population*/
    pop_circle.Evolve(rnd, SELECTION, crs, {&m1, &m2, &m3, &m4});
    pop_square.Evolve(rnd, SELECTION, crs, {&m1, &m2, &m3, &m4});
  };

  /* Evolving without elite */
  size_t checkpoint = size_t(N_GEN * 3 / 3);
  for (size_t i = 0; i < checkpoint; ++i)
    evolving(i);
  /* Evolving with elite */
  pop_circle.SetElite();
  pop_square.SetElite();
  for (size_t i = checkpoint; i < N_GEN; ++i)
    evolving(i);

  fout_bestcost[0] << N_GEN << "," << pop_circle.BestCost(0) << endl;
  fout_bestcost[1] << N_GEN << "," << pop_circle.BestCost(0.5) << endl;
  fout_bestcost[2] << N_GEN << "," << pop_square.BestCost(0) << endl;
  fout_bestcost[3] << N_GEN << "," << pop_square.BestCost(0.5) << endl;
  for (size_t i = 0; i < 4; ++i)
    fout_bestcost[i].close();

  /* Output best path */
  path_cities = {"out/091-bestpath_circle.csv", "out/091-bestpath_square.csv"};
  cities = {pop_circle.GetBest().ApplyTo(&dec_circle),
            pop_square.GetBest().ApplyTo(&dec_square)};

  for (size_t i = 0; i < 2; ++i) {
    ofstream &fout = fout_cities[i];
    string &path = path_cities[i];
    vector<City> &city = cities[i];

    fout.open(path);
    FileCheck(fout, path);
    fout << "x,y\n";
    for (auto x : city)
      fout << x.a << "," << x.b << endl;

    fout.close();
  }

  return 0;
}
