#include "tsp.h"

City::City(double x, double y) {
  a = x * x + y * y;
  b = -2 * x * y;
}

array<double, 2> City::GetCoords() {
  double t = 2 * a;
  double d = sqrt(t * t - b * b);
  return {sqrt(t - d), sqrt(d - a)};
}

/***************************************/

Chromosome::Chromosome(vector<City> db, Random &rnd)
    : path(db.size()), cost(0) {
  iota(path.begin(), path.end(), 0);
  for (int i = 1; i < path.size(); ++i) {
    swap(path[i], path[(int)rnd.Rannyu(1, path.size())]);
    cost += db.at(path[i]) - db.at(path[i - 1]);
  }
  cost += db.at(path.back()) - db.at(path.front());
}

void Chromosome::EvalCost(vector<City> db) {
  cost = db.at(path.back()) - db.at(path.front());
  for (int i = 1; i < path.size(); ++i)
    cost += db.at(path[i]) - db.at(path[i - 1]);
}

void Chromosome::MutationPermute(int i, int j) { swap(path.at(i), path.at(j)); }

void Chromosome::MutationCycle(int start, int end, int steps) {
  if ((start < 1) || (end >= path.size())) {
    cerr << "PROBLEM: index in Cycle does not fulfil requirements\n";
    exit(-1);
  }
  int middle = steps % (end - start) + start;
  rotate(path.begin() + start, path.begin() + middle, path.begin() + end);
}

void Chromosome::MutationMirror(int start, int end) {
  if ((start < 1) || (end >= path.size())) {
    cerr << "PROBLEM: index in Mirror does not fulfil requirements\n";
    exit(-1);
  }
  reverse(path.begin() + start, path.begin() + end);
}

void Chromosome::MutationPermuteInterval(int start1, int start2, int lenght) {
  if ((start1 < 1) || ((start1 + lenght) >= start2) ||
      ((start2 + lenght) > path.size())) {
    cerr << "PROBLEM: index in PermuteInterval does not fulfil requirements\n";
    exit(-1);
  }

  swap_ranges(path.begin() + start1, path.begin() + start1 + lenght,
              path.begin() + start2);
}

void Crossover(Chromosome &genitore1, Chromosome &genitore2, Chromosome &son1,
               Chromosome &son2, vector<City> db, Random &rnd) {
  const int N = genitore1.path.size();
  son1.path.reserve(N);
  son2.path.reserve(N);
  int cut_position = rnd.Rannyu(1, N - 1);
  for (int i = 0; i < cut_position; ++i) {
    son1[i] = genitore1[i];
    son2[i] = genitore2[i];
  }

  for (int i = 0, idx = cut_position; idx < N; ++i) {
    for (int j = idx; j < N; ++j) {
      if (genitore2[i] == genitore1[j]) {
        son1[idx++] = genitore1[j];
        break;
      }
    }
  }
  for (int i = 0, idx = cut_position; idx < N; ++i) {
    for (int j = idx; j < N; ++j) {
      if (genitore1[i] == genitore2[j]) {
        son2[idx++] = genitore2[j];
        break;
      }
    }
  }
  son1.EvalCost(db);
  son2.EvalCost(db);
}

/***************************************/

Population::Population(vector<City> db, int N) : p(N), igen(1) {
  for (int i = 0; i < N; ++i)
    p.emplace_back(db);

  this->Sort();
}

int Population::Select(Random &rnd) {
  int i = p.size() * pow(rnd.Rannyu(), selection_factor);
  return ++i;
}

int Population::Select(Random &rnd, int prev_i) {
  int i;
  do {
    i = p.size() * pow(rnd.Rannyu(), selection_factor) + 1;
  } while (i == prev_i);
  return i;
}

double Population::BestCost(double frac) {
  int imax = p.size() * frac;
  ++imax;
  double ave = 0;
  for (int i = 0; i < imax; ++i)
    ave += p[i].cost;
  return ave / imax;
}

void Population::Sort() { sort(p.begin(), p.end()); }

void Population::NextGeneration(vector<City> db, Random &rnd) {
  ++igen;
  vector<Chromosome> offspring(p.size());
  for (int i = 0; i < p.size(); i += 2) {
    // Crossover two parents
    int igenitore1 = Select(rnd);
    int igenitore2 = Select(rnd, igenitore1);
    if (rnd.Rannyu() < crossover)
      Crossover(p[igenitore1], p[igenitore2], offspring[i], offspring[i + 1],
                db, rnd);
    else {
      offspring[i] = p[igenitore1];
      offspring[i + 1] = p[igenitore2];
    }

    // Introducing mutation on sons
    for (int j = 0; j < 2; ++j) {
      if (rnd.Rannyu() < mutation) {
        switch ((int)rnd.Rannyu() * 4) {
        case 0: { // Permutation
          int m = rnd.Rannyu(1, offspring[i + j].Size());
          int n = 0;
          do {
            n = rnd.Rannyu(1, offspring[i + j].Size());
          } while (n == m);
          offspring[i + j].MutationPermute(m, n);
          break;
        }
        case 1: { // Permutation of intervals
          int m = rnd.Rannyu(1, offspring[i + j].Size() * 0.5 - 1);
          int n = rnd.Rannyu(offspring[i + j].Size() * 0.5,
                             offspring[i + j].Size() - 1);
          int maxlen = min((int)(offspring[i + j].Size() * 0.5) - m,
                           (int)offspring[i + j].Size() - n);
          int len = rnd.Rannyu(1, maxlen);
          offspring[i + j].MutationPermuteInterval(m, n, len);
          break;
        }
        case 2: { // Cycle interval
          int m = rnd.Rannyu(1, offspring[i + j].Size() - 2);
          int n = rnd.Rannyu(m, offspring[i + j].Size());
          int steps = rnd.Rannyu(1, offspring[i + j].Size());
          offspring[i + j].MutationCycle(m, n, steps);
          break;
        }
        case 3: { // Mirror interval
          int m = rnd.Rannyu(1, offspring[i + j].Size() - 2);
          int n = rnd.Rannyu(m, offspring[i + j].Size());
          offspring[i + j].MutationMirror(m, n);
          break;
        }
        }
      }
    }
  }
  p = offspring;
  Sort();
}
