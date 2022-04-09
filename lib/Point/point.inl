template <typename T, int N> point<T,N>::point(){
    //test: è array di zeri?
    p_={0};
}

template <typename T, int N> point<T,N>::~point(){}

template <typename T, int N> array<T,N> point<T,N>::GetPoint(){
    return p_;
}

template <typename T, int N> double point<T,N>::Lenght(){
    return sqrt(inner_product(p_.begin(),p_.end(),p_.begin(),0));
}
