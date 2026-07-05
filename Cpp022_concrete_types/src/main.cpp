// Concrete types in C++
#include <iostream>
#include <initializer_list>

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

// Containners as Concrete Types
class Vector {
private:
    double* elem;   // Pointer to heap memory
    int sz;         // Number of elements
public:
    // Constructor acquires resources
    Vector(int s) : elem(new double[s]), sz(s) {
        for(int i = 0; i != s; i++){
            elem[i] = 0;    // Intialize to zero
        }
        std::cout << "[CONSTRUCTOR] Vector Object Instantiated and Intialized to Zero" << std::endl;
    }

    // Initializer-list constructor - for intializing the container with a list of elements.
    Vector(std::initializer_list<double> lst)
        : elem{new double[lst.size()]}, sz{static_cast<int>(lst.size())}
    {
        // Copy the elements from the brace list into our heap memory
        std::copy(lst.begin(), lst.end(), elem);
        std::cout << "[CONSTRUCTOR] Vector Object Instantiated and initialized with element list shared" << std::endl;
    }

    // Destructor releases resources automatically!
    ~Vector(){
        delete[] elem;
        std::cout << "[DESTRUCTOR] Vector \'elem\' will be destroyed and memory will be freed!" << std::endl;
    }

    // Operator Overloading allows array-style access
    double& operator[](int i) {
        return elem[i];
    }

    int size() const {
        return sz;
    }
};

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

    Vector v(10);
    v[1] = 7;

    // when 'v' goes out of scope i.e. program ends, the Destructor (~Vector()) is called and it automatically frees memory.

    // Using the initializer_list constructor!
    // Beautiful, clean, C-style syntax, but completely memory-safe.
    Vector mySensorData = {24.5, 25.1, 26.8, 23.9};

    std::cout << "Read index 2: " << mySensorData[2] << std::endl;

    std::cout << "===================================================" << std::endl;
    std::cin.get();
}