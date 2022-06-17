#ifndef __Point__
#define __Point__

#include <array>
#include <cmath>
#include <cstdarg>
#include <functional>
#include <numeric>

using namespace std;

/* A point in the `N`-dimensional space with a (eventually
 * stochastic) evolution law `m_`. Automatically manages the
 * acceptance rate of the move  */

template <typename T, size_t N> class point {
public:
  point(function<bool(point<T, N> &)> move, array<T, N> x0 = {0});
  ~point();

  T &operator[](size_t i) { return p_[i]; }
  array<T, N> &ToArray() { return p_; }

  /* LA USO?! */
  void Reset();
  void Move();
  void Jump(array<T, N>);
  double Lenght2();
  double Lenght();
  double Rate();

private:
  array<T, N> p_;
  function<bool(point<T, N> &)> move_;
  int attempted_, accepted_;
};

template <typename T, size_t N>
point<T, N>::point(function<bool(point<T, N> &)> move, array<T, N> x0) {
  p_ = x0;
  move_ = move;
  accepted_ = 0;
  attempted_ = 0;
}

template <typename T, size_t N> point<T, N>::~point() {}

template <typename T, size_t N> void point<T, N>::Reset() {
  accepted_ = 0;
  attempted_ = 0;
}

template <typename T, size_t N> void point<T, N>::Jump(array<T, N> p) {
  p_ = p;
}

template <typename T, size_t N> void point<T, N>::Move() {
  accepted_ += move_(*this);
  attempted_++;
}

template <typename T, size_t N> double point<T, N>::Lenght2() {
  double l = 0;
  for (T x : p_)
    l += x * x;
  return l;
}
template <typename T, size_t N> double point<T, N>::Lenght() {
  return sqrt(Lenght2());
}

template <typename T, size_t N> double point<T, N>::Rate() {
  return (double)accepted_ / (double)attempted_;
}

#endif
