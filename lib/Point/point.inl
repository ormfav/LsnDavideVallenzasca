#include <cmath>

template <typename T, int N> point<T,N>::point(function<bool(point<T,N>&)> move){
    p_={0};
    move_=move;
    accepted_=0;
    attempted_=0;
}

template <typename T, int N> point<T,N>::~point(){}

template <typename T, int N> void point<T,N>::Reset(){
    for(T& x : p_) x=0;
}

template <typename T, int N> void point<T,N>::Move(){
    accepted_+=move_(this->p_);
    attempted_++;
}

template <typename T, int N> double point<T,N>::Lenght(){
    return sqrt(inner_product(p_.begin(),p_.end(),p_.begin(),0));
}

template <typename T, int N> double point<T,N>::Rate(){
    return (double)accepted_/(double)attempted_;
}
