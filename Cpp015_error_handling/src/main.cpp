// Error Handling in C++
#include <iostream>
#include <stdexcept> // Standard C++ exception types
#include <cstdint>

// A mock function that might fail
int readSensorData(int pin) {
    if (pin < 0 || pin > 15) {
        // We throw an exception object instead of returning -1
        throw std::invalid_argument("Hardware error: Invalid pin number!");
    }
    
    return 1024; // Simulated valid reading
}

class NetworkSocket {
public:
    NetworkSocket(int port) {
        if (port < 1024) {
            throw std::runtime_error("Cannot bind to privileged port!");
        }
        // Bind port...
    }
};

// Simulating a memory-mapped hardware register configuration
struct SpiRegisters {
    uint32_t control;
    uint32_t status;
    uint32_t data;
};

// The compiler checks this before building the binary!
// If someone adds a variable to SpiRegisters, the build will instantly fail.
static_assert(sizeof(SpiRegisters) == 12, "FATAL: SpiRegisters struct must be exactly 12 bytes to match hardware!");


int main() {
    // Error Handling Implementation in genric use case like function call
    try {
        // The compiler attempts to run this code
        std::cout << "Reading pin 5: " << readSensorData(5) << std::endl;
        
        // This will trigger the exception!
        std::cout << "Reading pin 99: " << readSensorData(99) << std::endl; 
        
        // This line will NEVER execute because the line above threw an exception.
        std::cout << "All sensors read successfully." << std::endl;  
    } catch (const std::invalid_argument& e) {
        // If an invalid_argument exception is thrown anywhere in the 'try' block, 
        // the CPU jumps immediately here.
        std::cout << "CAUGHT EXCEPTION: " << e.what() << std::endl;
    }

    // Error Handling Implementation in Classes: Constructor Problem in a Class
    try {
        NetworkSocket mySock(80); // Fails to construct! Throws exception.
    } catch (const std::runtime_error& e) {
        std::cout << "Failed to start server: Caught Exception: " << e.what() << '\n';
    }
    
    return 0;
}