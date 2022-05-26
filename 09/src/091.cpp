#include "../../lib/Misc/misc.h"
#include "../../lib/Random/random.h"
#include "../../lib/Tsp/tsp.h"
#include <iostream>
#include <set>

using namespace std;

/* #include "../in/091-conf.inl" */
#define N_CITIES 34
#define N_GEN 20
#define INDIVIDUALS 50

int main(int argc, char *argv[]) {
  Random rnd("lib/Random/Primes", "lib/Random/seed.in");

  /* Cities on a circumference */
  set<double> angles;
  do {
    angles.emplace(rnd.Rannyu() * 2 * M_PI);
  } while (angles.size() != N_CITIES);
  vector<City> circle;
  circle.reserve(N_CITIES);
  for (double x : angles)
    circle.emplace_back(cos(x), sin(x));

  /* ofstream fout_circle; */
  /* string path_circle="09/out/091-coord_circle.csv"; */
  /* FileCheck(fout_circle, path_circle); */
  /* fout_circle.open(path_circle); */
  /* fout_circle<<"x,y\n"; */
  /* for(auto x : circle){ */
  /*   array<double,2> c = x.GetCoords(); */
  /*   fout_circle<<c[0]<<","<<c[1]<<endl; */
  /* } */
  Population tsp_circle(circle, INDIVIDUALS, rnd, 3, 9. / 100, 9. / 20);

  /* Cities in a square */
  set<City, CityCompare> aux;
  do {
    aux.emplace(rnd.Rannyu(), rnd.Rannyu());
  } while (aux.size() != N_CITIES);
  vector<City> square(aux.begin(), aux.end());

  Population tsp_square(square, INDIVIDUALS, rnd, 3, 20. / 100, 3. / 5);

  array<ofstream, 2> fout;
  array<string, 2> path = {"09/out/091-costs_circle.csv",
                           "09/out/091-costs_square.csv"};
  for (int i = 0; i < 2; ++i) {
    fout[i].open(path[i]);
    FileCheck(fout[i], path[i]);
    fout[i] << "best,average_best_half\n";
  }

  for (int i = 0; i < N_GEN; ++i) {
    tsp_circle.NextGeneration(circle, rnd);
    tsp_square.NextGeneration(square, rnd);
    fout[0] << tsp_circle.BestCost(0) << "," << tsp_circle.BestCost(0.5)
            << endl;
    fout[1] << tsp_square.BestCost(0) << "," << tsp_square.BestCost(0.5)
            << endl;
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
