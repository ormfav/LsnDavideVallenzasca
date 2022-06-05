#ifndef __City__
#define __City__

#include <vector>

using namespace std;

struct City {
  City(double, double);
  bool operator<(const City &c) { return a < c.a; };
  double operator-(const City &c) {
    return (a - c.a) * (a - c.a) - (b - c.b) * (b - c.b);
  };

  double a, b;
};

City::City(double x, double y) {
  a = x;
  b = y;
}

double costfunction(vector<City> v) {
  double cost = v.back() - v.front();
  for (size_t i = 0; i < v.size(); ++i)
    cost += v[i + 1] - v[i];
  return cost;
};

#endif
