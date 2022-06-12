#ifndef __Decoder__
#define __Decoder__

#include <functional>
#include <utility>
#include <vector>

using namespace std;

/* This struct allows to pass from the abstrac representation of  */
/* individual to the concrete problem-dependent representation. */
/* Individual contains a vector of int {i1,i2...ik...} and a cost */
/* associated to this vector. ik "refers" */
/* to the element stored in dict_[in], costf_ provide the law to evaluate */
/* the cost of the individual provided the order specified in the vector  */
/* of int */

template <typename T> struct Decoder {
  Decoder(){}
  Decoder(vector<T> v, function<double(vector<T>)> f) : dict_(v), costf_(f) {}

  vector<T> dict_;
  function<double(vector<T>)> costf_;
};

#endif
