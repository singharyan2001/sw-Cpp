// virtual Functions in C++

#include <iostream>

/* 
 * Virtual Function Implementation Config Macro.
 * if enabled, the example would show how virtual function is used
 * if diabled, the example would show why using pointers would create an issue, and not achieve the intended functionality
*/
#define VIRTUAL_EXAMPLE 1

#if VIRTUAL_EXAMPLE     // Virtual function implementation
class Peripheral {
public:
    // Virtual Tells the compiler: "Check the VTable at runtime!"
    virtual void transmit() {
        std::cout << "Sending generic peripheral data..." << std::endl;
    }
};

class SPI : public Peripheral {
public:
    // override tells the compiler: "Make sure i am actually overriding a virtual base function"
    void transmit() override {
        std::cout << "Sending fast SPI data!" << std::endl;
    }
};
#else               // Pointer based Implementation
class Peripheral {
public:
    void transmit() {
        std::cout << "Sending generic peripheral data..." << std::endl;
    }
};

class SPI : public Peripheral {
public:
    void transmit() {
        std::cout << "Sending fast SPI data!" << std::endl;
    }
};
#endif

int main(){
    std::cout << "=========== TOPIC: VIRTUAL FUNCTIONS IN C++ ===========" << std::endl;

    SPI mySpibus;
    std::cout << "SIZE OF mySpibus: " << sizeof(mySpibus) << " Bytes" << std::endl;
    // Create a base pointer pointing to the derived object
    Peripheral* devPtr = &mySpibus;
    // What happens here?
    devPtr->transmit();

    std::cout << "=======================================================" << std::endl;
    std::cin.get();
}