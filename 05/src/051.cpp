#include "../../lib/DataBlocking/include/datablocking.h"
#include "../../lib/Metropolis/include/metropolis.h"
#include "../../lib/Misc/include/misc.h"
#include "../../lib/Point/include/point.h"
#include "../../lib/Random/include/random.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>

using namespace std;
using pt3D = point<double, 3>;

#include "../in/051-datablocking-conf.inl"
#include "../in/051-metropolis-conf.inl"

int main(int argc, char *argv[]) {
  /* Initializing random number generator */
  Random rnd("in/Primes", "in/seed.in");

  /* Preparing metropolis */
  auto psi2_100_ratio = [](pt3D q, pt3D p) {
    double dl = p.Lenght() - q.Lenght();
    return exp(2. * dl);
  };
  auto psi2_210_ratio = [](pt3D q, pt3D p) {
    double dl = p.Lenght() - q.Lenght();
    double z = q[2] / p[2];
    return z * z * exp(dl);
  };
  function<double(pt3D)> aver = [](pt3D p) { return p.Lenght(); };

  mrt2<3, 'u'> metro100_unif(DELTA_100, psi2_100_ratio, rnd);
  mrt2<3, 'g'> metro100_gaus(SIGMA_100, psi2_100_ratio, rnd);
  mrt2<3, 'u'> metro210_unif(DELTA_210, psi2_210_ratio, rnd);
  mrt2<3, 'g'> metro210_gaus(SIGMA_210, psi2_210_ratio, rnd);

  array<double, 3> p0_100 = {0, 0, 0};
  array<double, 3> p0_210 = {0, 0, 2};

  array<ofstream, 4> fout_equi;
  array<string, 4> path_equi = {
      "out/051-equi_100_unif.csv", "out/051-equi_100_gaus.csv",
      "out/051-equi_210_unif.csv", "out/051-equi_210_gaus.csv"};
  for (size_t i = 0; i < 4; ++i) {
    fout_equi[i].open(path_equi[i]);
    FileCheck(fout_equi[i], path_equi[i]);
  }

  cout << "Orbital: 100. Proposed step: unif\n";
  cout << "Acceptance: "
       << metro100_unif.Equilibrate(p0_100, 500, aver, fout_equi[0]) << endl;
  cout << "Orbital: 100. Proposed step: gauss\n";
  cout << "Acceptance: "
       << metro100_gaus.Equilibrate(p0_100, 500, aver, fout_equi[1]) << endl;
  cout << "Orbital: 210. Proposed step: unif\n";
  cout << "Acceptance: "
       << metro100_unif.Equilibrate(p0_210, 500, aver, fout_equi[2]) << endl;
  cout << "Orbital: 210. Proposed step: gauss\n";
  cout << "Acceptance: "
       << metro100_gaus.Equilibrate(p0_210, 500, aver, fout_equi[3]) << endl;

  for (auto &file : fout_equi)
    file.close();

  /* Initializing data blocking */
  array<function<double(pt3D)>, 1> db_eval = {aver};

  array<dataBlocks<3, 1>, 4> r = {
      dataBlocks<3, 1>(STEPS_PER_BLOCK, metro100_unif, db_eval, p0_100),
      dataBlocks<3, 1>(STEPS_PER_BLOCK, metro100_gaus, db_eval, p0_100),
      dataBlocks<3, 1>(STEPS_PER_BLOCK, metro210_unif, db_eval, p0_210),
      dataBlocks<3, 1>(STEPS_PER_BLOCK, metro210_gaus, db_eval, p0_210)};

  /* Initializing output files */
  array<array<ofstream, 1>, 4> fout_r;
  array<string, 4> path_r = {"out/051-r_100_unif.csv", "out/051-r_100_gaus.csv",
                             "out/051-r_210_unif.csv",
                             "out/051-r_210_gaus.csv"};

  array<ofstream, 4> fout_points;
  array<string, 4> path_points = {
      "out/051-points_100_unif.csv", "out/051-points_100_gaus.csv",
      "out/051-points_210_unif.csv", "out/051-points_210_gaus.csv"};

  for (size_t i = 0; i < 4; ++i) {
    fout_r[i][0].open(path_r[i]);
    FileCheck(fout_r[i][0], path_r[i]);
    fout_points[i].open(path_points[i]);
    FileCheck(fout_points[i], path_points[i]);
  }

  /* Data blocking */
  for (size_t i = 0; i < N_BLOCKS; ++i) {
    cout << "--- Block " << (i + 1) << " ---\n";
    for (size_t j = 0; j < 4; ++j) {
      cout << path_r[j].substr(8, 10) + " acceptance: "
           << r[j].Measure(100, fout_points[j]) << endl;
      r[j].EvalBlock(fout_r[j]);
    }
    cout << endl;
  }

  for (auto &file : fout_r)
    file[0].close();
  for (auto &file : fout_points)
    file.close();

  return 0;
}
