#include "../../lib/Ga/crossovers.h"
#include "../../lib/Ga/decoder.h"
#include "../../lib/Ga/individual.h"
#include "../../lib/Ga/mutations.h"
#include "../../lib/Ga/population.h"

#include "city.h"

#include "../../lib/Misc/misc.h"
#include "../../lib/Random/random.h"

#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

/* #include "../in/091-conf.inl" */
#define N_CITIES 34
#define N_GEN 20
#define INDIVIDUALS 50

#define SELECTION 3
#define P_CROSSOVER 0.4
#define P_PERMELM 0.05
#define P_PERMINT 0.05
#define P_CYCLEINT 0.05
#define P_MIRRORINT 0.05

int main(int argc, char *argv[]) {
  Random rnd("lib/Random/Primes", "lib/Random/seed.in");

  /* Cities on a circumference */
  Decoder<City> circle;
  circle.costf_ = costfunction;
  set<double> angles;
  do {
    angles.emplace(rnd.Rannyu() * 2 * M_PI);
  } while (angles.size() != N_CITIES);
  circle.dict_.reserve(N_CITIES);
  for (double x : angles)
    circle.dict_.emplace_back(cos(x), sin(x));

  /* Cities on a square */
  Decoder<City> square;
  circle.costf_ = costfunction;
  set<City> aux;
  do {
    aux.emplace(rnd.Rannyu(), rnd.Rannyu());
  } while (aux.size() != N_CITIES);
  square.dict_.assign(aux.begin(), aux.end());

  /* Core of the GA */
  Population<City> pop_circle(INDIVIDUALS, circle, rnd);
  Population<City> pop_square(INDIVIDUALS, square, rnd);
  for (size_t i = 0; i < N_GEN; ++i) {
    /* Building crossover */
    fixHead crs(P_CROSSOVER, rnd.Rannyu(1, N_CITIES - 1));

    int x, y, z;
    /* Building mutations */
    x = rnd.Rannyu(1, N_CITIES);
    do {
      y = rnd.Rannyu(1, N_CITIES);
    } while (x == y);
    permuteElements m1(P_PERMELM, x, y);

    x = rnd.Rannyu(1, N_CITIES * 0.5);
    y = rnd.Rannyu(x, N_CITIES);
    z = rnd.Rannyu(1, min(y - x, N_CITIES));
    permuteIntervals m2(P_PERMINT, x, y, z);

    x = rnd.Rannyu(1, N_CITIES - 2);
    y = rnd.Rannyu(x, N_CITIES);
    z = rnd.Rannyu(1, N_CITIES);
    cycleInterval m3(P_CYCLEINT, x, y, z);

    x = rnd.Rannyu(1, N_CITIES - 2);
    y = rnd.Rannyu(x, N_CITIES);
    mirrorInterval m4(P_MIRRORINT, x, y);

    /* Evolving population*/
    pop_circle.Evolve(rnd, SELECTION, crs, {m1, m2, m3, m4});
    pop_square.Evolve(rnd, SELECTION, crs, {m1, m2, m3, m4});
  }

  /* ofstream fout_circle; */
  /* string path_circle="09/out/091-coord_circle.csv"; */
  /* FileCheck(fout_circle, path_circle); */
  /* fout_circle.open(path_circle); */
  /* fout_circle<<"x,y\n"; */
  /* for(auto x : circle){ */
  /*   array<double,2> c = x.GetCoords(); */
  /*   fout_circle<<c[0]<<","<<c[1]<<endl; */
  /* } */

  /* Cities in a square */
  set<City> aux;
  do {
    aux.emplace(rnd.Rannyu(), rnd.Rannyu());
  } while (aux.size() != N_CITIES);
  vector<City> square(aux.begin(), aux.end());

  array<ofstream, 2> fout;
  array<string, 2> path = {"09/out/091-costs_circle.csv",
                           "09/out/091-costs_square.csv"};
  for (int i = 0; i < 2; ++i) {
    fout[i].open(path[i]);
    FileCheck(fout[i], path[i]);
    fout[i] << "best,average_best_half\n";
  }

  /* ofstream fout_circle, fout_square; */
  /* string path_square = "09/out/091-coord_square.csv", */
  /*        path_circle = "09/out/091-coord_circle.csv"; */
  /* FileCheck(fout_circle, path_circle); */
  /* FileCheck(fout_square, path_square); */
  /* fout_circle.open(path_circle); */
  /* fout_circle << "x,y\n"; */
  /* fout_square.open(path_square); */
  /* fout_square << "x,y\n"; */
  /* for (int i=0;i<N_CITIES;++i) { */
  /*   array<double, 2> c = tsp_circle.p */
  /*   fout_square << c[0] << "," << c[1] << endl; */
  /* } */

  return 0;
}
