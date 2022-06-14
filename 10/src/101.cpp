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
#include <cstdlib>
#include <iostream>
#include <set>

using namespace std;

#include "../in/10-conf_ga.inl"
#include "../in/10-conf_simulation.inl"

int main(int argc, char *argv[]) {
  Random rnd("in/Primes", "in/seed.in");

  /* Building decoder from file */
  Decoder<City> dec;
  dec.costf_ = costfunction;
  string path = "in/capitals.dat";
  ifstream fin(path);
  FileCheck(fin, path);
  string lon, lat;
  fin >> lon >> lat;
  while (fin >> lon >> lat) {
    dec.dict_.emplace_back(stod(lon), stod(lat));
  }
  fin.close();
  const int ncities = dec.dict_.size();

  Population<City> pop(INDIVIDUALS, dec, rnd);

  /* Output first path */
  path = "out/101-firstpath.csv";
  ofstream fout(path);
  FileCheck(fout, path);

  fout << "x,y\n";
  for (auto x : pop.GetBest().ApplyTo(&dec))
    fout << x.a << "," << x.b << endl;

  fout.close();

  /* Preparing output costs */
  array<ofstream, 2> fout_bestcost;
  array<string, 2> path_bestcost = {"out/101-bestcost.csv",
                                    "out/101-besthalfcost.csv"};

  for (size_t i = 0; i < 2; ++i) {
    fout_bestcost[i].open(path_bestcost[i]);
    FileCheck(fout_bestcost[i], path_bestcost[i]);
    fout_bestcost[i] << "gen,cost\n";
  }

  /* We are performing two separate loops: using lambda to avoid
   * writing the same code twice */
  auto evolving = [&](size_t i) {
    cout << "Generation: " << (i + 1) << "/" << N_GEN << endl;
    /* Output costs */
    fout_bestcost[0] << i << "," << pop.BestCost(0) << endl;
    fout_bestcost[1] << i << "," << pop.BestCost(0.5) << endl;

    /* Building crossover */
    fixHead crs(P_CROSSOVER, rnd.Rannyu(1, ncities - 1));

    int x, y, z;
    /* Building mutations */
    x = rnd.Rannyu(1, ncities);
    do {
      y = rnd.Rannyu(1, ncities);
    } while (x == y);
    permuteElements m1(P_PERMELM, x, y);

    x = rnd.Rannyu(1, int(ncities * 0.5));
    y = rnd.Rannyu(x, ncities);
    z = rnd.Rannyu(1, min(y - x, ncities - y));
    permuteIntervals m2(P_PERMINT, x, y, z);

    x = rnd.Rannyu(1, ncities - 2);
    y = rnd.Rannyu(x + 1, ncities);
    z = rnd.Rannyu(1, ncities);
    cycleInterval m3(P_CYCLEINT, x, y - x, z);

    x = rnd.Rannyu(1, ncities - 2);
    y = rnd.Rannyu(x, ncities);
    mirrorInterval m4(P_MIRRORINT, x, y - x);

    /* Evolving population*/
    pop.Evolve(rnd, SELECTION, crs, {&m1, &m2, &m3, &m4});
  };

  /* Evolving without elite */
  for (size_t i = 0; i < size_t(N_GEN * 2 / 3); ++i)
    evolving(i);
  /* Evolving with elite */
  pop.SetElite();
  for (size_t i = size_t(N_GEN * 2 / 3) + 1; i < N_GEN; ++i)
    evolving(i);

  fout_bestcost[0] << N_GEN << "," << pop.BestCost(0) << endl;
  fout_bestcost[1] << N_GEN << "," << pop.BestCost(0.5) << endl;
  for (size_t i = 0; i < 2; ++i)
    fout_bestcost[i].close();

  /* Output best path */
  vector<City> cities = {pop.GetBest().ApplyTo(&dec)};
  path = "out/101-bestpath.csv";
  fout.open(path);
  FileCheck(fout, path);

  fout << "x,y\n";
  for (auto x : cities)
    fout << x.a << "," << x.b << endl;

  fout.close();

  return 0;
}
