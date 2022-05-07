#ifndef __Point__
#define __Point__

#include <array>
#include <functional>
#include <numeric>
#include <cmath>

using namespace std;

template<typename T, int N> class point
{
public:
    point(function<bool(array<T,N>&)> move);
    ~point();

    T& operator[](int i){return p_[i];}

    void Reset();
    void Move();
    double Lenght();
    double Rate();

private:
    array<T,N> p_; 
    function<bool(array<T,N>&)> move_;
    int attempted_, accepted_;
};

 #include "point.inl" 

#endif
