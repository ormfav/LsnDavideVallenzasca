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

#include "mpi.h"

using namespace std;

#include "../in/10-conf_ga.inl"
#include "../in/10-conf_simulation.inl"

int main(int argc, char *argv[]) {
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  Random rnd("in/Primes", "in/seed.in", rank + 1);

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

  /* Preparing output costs and storing first path: only best
   * continent will output */
  array<vector<double>, 2> bestcost;
  Individual firstpath = pop.GetBest();

  /* We are performing two separate loops: using lambda to avoid
   * writing the same code twice */
  auto evolving = [&](size_t index) {
    /* Output costs */
    bestcost[0].push_back(pop.BestCost(0));
    bestcost[1].push_back(pop.BestCost(0.5));

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

    /* Migration */
    if (index % N_MIGRATION == 0) {
      int sender, receiver;
      vector<int> sendto, receivefrom;
      vector<int> in_chromo(pop.GetBest().Size(), 0);
      vector<int> out_chromo(pop.GetBest().Begin(), pop.GetBest().End());
      double in_cost, out_cost = pop.GetBest().GetCost();

      if (rank == 0) {
        /* node=vector_index sends-to/receives-from node=vector_value */
        sendto.reserve(size);
        for (int i = 0; i < size; ++i)
          sendto.push_back(i);
        receivefrom = sendto;
        for (int i = 0; i < int(size * 0.5)+5; ++i)
          swap(sendto[int(rnd.Rannyu() * sendto.size())],
               sendto[int(rnd.Rannyu() * sendto.size())]);
        for (size_t i = 0; i < receivefrom.size(); ++i)
          receivefrom[sendto[i]] = i;
      }

      MPI_Scatter(sendto.data(), 1, MPI_INTEGER, &receiver, 1, MPI_INTEGER, 0,
                  MPI_COMM_WORLD);
      MPI_Scatter(receivefrom.data(), 1, MPI_INTEGER, &sender, 1, MPI_INTEGER,
                  0, MPI_COMM_WORLD);

      MPI_Request request;
      int tag_chromosome = 0;
      int tag_cost = 1;
      MPI_Isend(out_chromo.data(), out_chromo.size(), MPI_INTEGER, receiver,
                tag_chromosome, MPI_COMM_WORLD, &request);
      MPI_Isend(&out_cost, 1, MPI_DOUBLE, receiver, tag_cost, MPI_COMM_WORLD,
                &request);
      MPI_Irecv(in_chromo.data(), in_chromo.size(), MPI_INTEGER, sender,
                tag_chromosome, MPI_COMM_WORLD, &request);
      MPI_Irecv(&in_cost, 1, MPI_DOUBLE, sender, tag_cost, MPI_COMM_WORLD,
                &request);

      MPI_Barrier(MPI_COMM_WORLD);

      for (size_t i = 0; i < pop.GetBest().Size(); ++i)
        pop.GetBest()[i] = in_chromo[i];
      pop.GetBest().GetCost() = in_cost;
      pop.Sort();
    }
  };

  size_t checkpoint = size_t(N_GEN * 3 / 3);
  /* Evolving without elite */
  cout << "Performing evolution without elite on node: " << rank << endl;
  for (size_t i = 0; i < checkpoint; ++i)
    evolving(i);

  /* Evolving with elite */
  cout << "Performing evolution with elite on node: " << rank << endl;
  pop.SetElite();
  for (size_t i = checkpoint; i < N_GEN; ++i)
    evolving(i);

  bestcost[0].push_back(pop.BestCost(0));
  bestcost[1].push_back(pop.BestCost(0.5));

  /* Collect cost and rank to node 0 */
  cout << "Gathering all results\n";

  struct {
    double cost;
    int rk;
  } in, out;
  out.cost = bestcost[0].back();
  out.rk = rank;
  MPI_Allreduce(&out, &in, 1, MPI_DOUBLE_INT, MPI_MINLOC, MPI_COMM_WORLD);

  /* Output operations for node with best results */
  if (rank == in.rk) {
    /* Output first path */
    path = "out/103-firstpath.csv";
    ofstream fout(path);
    FileCheck(fout, path);

    fout << "x,y\n";
    for (auto x : firstpath.ApplyTo(&dec))
      fout << x.a << "," << x.b << endl;

    fout.close();

    /* Output best path */
    path = "out/103-bestpath.csv";
    fout.open(path);
    FileCheck(fout, path);

    fout << "x,y\n";
    for (auto x : pop.GetBest().ApplyTo(&dec))
      fout << x.a << "," << x.b << endl;

    fout.close();

    array<string, 2> path_bestcost = {"out/103-bestcost.csv",
                                      "out/103-besthalfcost.csv"};
    array<ofstream, 2> fout_bestcost;

    for (size_t i = 0; i < 2; ++i) {
      fout_bestcost[i].open(path_bestcost[i]);
      FileCheck(fout_bestcost[i], path_bestcost[i]);
      fout_bestcost[i] << "gen,cost\n";
      for (size_t j = 0; j < bestcost[i].size(); ++j)
        fout_bestcost[i] << (j + 1) << "," << bestcost[i].at(j) << endl;
    }
  }

  MPI_Finalize();

  return 0;
}
