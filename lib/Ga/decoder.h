#ifndef __Decoder__
#define __Decoder__

#include <functional>
#include <utility>
#include <vector>

using namespace std;

template <typename T> struct Decoder {
  vector<T> dict;
  function<double(vector<T>)> costf;
};

#endif
