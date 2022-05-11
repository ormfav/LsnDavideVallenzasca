#ifndef __Point__
#define __Point__

#include <array>
#include <functional>
#include <numeric>
#include <cmath>
#include <cstdarg>

using namespace std;

/* ---- */
/* Head */
/* ---- */

template<typename T, int N> 
class point
{
public:
    point(function<bool(point<T,N>&)> move);
    point(array<T,N> x0, function<bool(point<T,N>&)> move);
    ~point();

    T& operator[](int i){return p_[i];}
    array<T,N>& ToArray(){return p_;}

    void Reset();
    void Move();
    void Jump(array<T,N>);
    double Lenght2();
    double Lenght();
    double Rate();

private:
    array<T,N> p_; 
    function<bool(point<T,N>&)> move_;
    int attempted_, accepted_;
};
template<typename T, int N>
point<T,N> ToPoint(T x1,...);


/* -------------- */
/* Implementation */
/* -------------- */

template <typename T, int N> 
point<T,N>::point(function<bool(point<T,N>&)> move){
    p_={0};
    move_=move;
    accepted_=0;
    attempted_=0;
}

template <typename T, int N>
point<T,N>::point(array<T,N> x0, function<bool(point<T,N>&)> move){
    p_=x0;
    move_=move;
    accepted_=0;
    attempted_=0;
}

template <typename T, int N>
point<T,N>::~point(){}

template <typename T, int N>
void point<T,N>::Reset(){
    for(T& x : p_) x=0;
}

template <typename T, int N>
void point<T,N>::Jump(array<T,N> p){
    p_=p;
}

template <typename T, int N>
void point<T,N>::Move(){
    accepted_+=move_(*this);
    attempted_++;
}

template <typename T, int N>
double point<T,N>::Lenght2(){
    return inner_product(p_.begin(),p_.end(),p_.begin(),0);
}
template <typename T, int N>
double point<T,N>::Lenght(){
    return sqrt(this->Lenght2());
}

template <typename T, int N>
double point<T,N>::Rate(){
    return (double)accepted_/(double)attempted_;
}

//DA TESTARE
template <typename T, int N>
point<T,N> ToPoint(function<bool(point<T,N>&)> m, ...){
    array<T,N> elems;
    va_list arg_list;
    va_start(arg_list, m);
    for(T& x : elems) x=va_arg(arg_list, T);
    va_end(arg_list);

    point<T,N> p(elems, m); 

    return p;
}

#endif
