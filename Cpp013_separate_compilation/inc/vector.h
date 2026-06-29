// Vector.hpp
#ifndef VECTOR_H
#define VECTOR_H

class Vector{
private:
    double* elem;   // elem points to an array of sz double
    int sz;
public:
    Vector(int s);
    double& operator[](int i);
    int size();
};

#endif // VECTOR_H