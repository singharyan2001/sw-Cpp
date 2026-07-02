// Concrete types in C++
#include <iostream>

class ComplexNumber {
private:
public:
    double real, imag;
    // 1. Fully initalizes the objects
    ComplexNumber(double r, double i) : real(r) , imag(i) {}

    // 2. operator Overloading makes it feel like an `int`
    ComplexNumber operator+(const ComplexNumber& other) const {
        return ComplexNumber(real + other.real, imag + other.imag);
    }

    bool operator==(const ComplexNumber& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const ComplexNumber& other) const {
        return !(*this == other);
    }
};

// Global Operator Overloading for printing my complex number concrete class
std::ostream& operator<<(std::ostream& stream, const ComplexNumber& cn){
    stream << "real: " << cn.real << " , " << "imag: " << cn.imag;
    return stream;
}

int main(){
    std::cout << "========== TOPIC: COMPLEX NUMBERS IN C++ ==========" << std::endl;

    // Look how it behaves exactly like an `int`!
    ComplexNumber a(1.0, 2.0);  // Placed on the stack
    ComplexNumber b(3.0, 4.0);

    ComplexNumber c = a + b;    // Independent Copy
    std::cout << "Complex Number C: " << c << std::endl;

    if(a == b){
        std::cout << "A == B ?? >> TRUE" << std::endl;
    }
    else{
        std::cout << "A == B ?? >> FALSE" << std::endl;
    }

    if(b != c){
        std::cout << "B != C ?? >> TRUE" << std::endl;
    }
    else{
        std::cout << "B != C ?? >> FALSE" << std::endl;
    }

    std::cout << "===================================================" << std::endl;
    std::cin.get();
}