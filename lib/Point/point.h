#include <array>
#include <functional>
#include <numeric>
#include <cmath>

using namespace std;

template<typename T, int N> class point
{
public:
    point();
    ~point();

    T& operator[](int i){return p_[i];}

    array<T,N> GetPoint();
    double Lenght();

private:
    array<T,N> p_; 
};

#include "point.inl"
