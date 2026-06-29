// Sepearate COmpilation in C++
#include <iostream>
#include <stdint.h>

#include "vector.h" // Get Vector's Interface
#include <cmath>    // Get the standard library math function interfaces

double sqrt_sum(Vector& v){
    double sum = 0;
    for(int i=0; i != v.size(); i++){
        sum += sqrt(v[i]);  // Sum of square roots
    }
    return sum;
}

int main(){
    std::cout << "======== Vector example to show header & source file implementation in C++ ========" << std::endl;
    Vector myVec(10);
    myVec[3] = 44.5;
    for(int i=0; i < 10; i++){
        std::cout << "Value in vector class array: " << myVec[i] << std::endl;
    }

    double sum = sqrt_sum(myVec);
    std::cout << "The Sum of sqrt root for myVec is: " << sum << std::endl;

    std::cout << "==================================" << std::endl;
    std::cin.get();
}