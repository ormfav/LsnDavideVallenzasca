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

    array<T,N> GetPoint();
    void Evolve(function<point<T,N>(point<T,N>)>);
    void Restart();
    double Lenght();

private:
    array<T,N> p_; 
};


template <typename T, int N> point<T,N>::point(){
    //test: è array di zeri?
    p_={0};
}

template <typename T, int N> point<T,N>::~point(){}

template <typename T, int N> array<T,N> point<T,N>::GetPoint(){
    return p_;
}

template <typename T, int N> void point<T,N>::Evolve(function<point<T,N>(point<T,N>)> f){
    p_=f(this).GetPoint();//è corretto?
}

template <typename T, int N> void point<T,N>::Restart(){
    for(T x : p_) x=0;
}

template <typename T, int N> double point<T,N>::Lenght(){
    return sqrt(inner_product(p_.begin(),p_.end(),p_.begin(),0));
}
