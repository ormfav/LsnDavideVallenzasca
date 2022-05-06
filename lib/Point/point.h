#include <array>
#include <functional>
#include <numeric>
#include <cmath>

using namespace std;

template<typename T, int N> class point
{
public:
    point(function<bool(point<T,N>&)> move);
    ~point();

    T& operator[](int i){return p_[i];}

    void Reset();
    void Move();
    double Lenght();
    double Rate();

private:
    array<T,N> p_; 
    function<bool(point<T,N>&)> move_;
    int attempted_, accepted_;
};

#include "point.inl"

