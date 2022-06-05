#ifndef __Decoder__
#define __Decoder__

#include <functional>
#include <utility>
#include <vector>

using namespace std;

template <typename T> struct Decoder {
  Decoder(){}
  Decoder(vector<T> v, function<double(vector<T>)> f) : dict_(v), costf_(f) {}

  vector<T> dict_;
  function<double(vector<T>)> costf_;
};

#endif
