// Object Instantiation in C++
#include <iostream>

class UartDriver {
public:
    UartDriver(){
        std::cout << "UART Booting..." << std::endl;
    }

    ~UartDriver(){
        std::cout << "UART Shutting down..." << std::endl;
    }

    void send(){
        std::cout << "Data Sent via UART" << std::endl;
    }
};

void transmitData(){
    // 1. Instantiation: Object created on the Stack. Constructor runs IMMEDIATELY.
    UartDriver myUart;

    // 2. Use the Object (using the standard dot '.' operator)
    myUart.send();

    // 3. The function ends.
    // The CPU automatically pops myUart off the stack.
    // The Destructor (~UartDriver) runs AUTOMATICALLY right here!
}

UartDriver* createPersistentDriver(){
    // 1. Instantiation: Ask for heap memory and run the Constructor
    UartDriver* heapUart = new UartDriver();

    // 2. Use the object (using the arrow '->' operator because it's a pointer)
    heapUart->send();

    // 3. the function ends, BUT the object survives!
    return heapUart;
}

int main(){
    std::cout << "========== Topic: Object Instantiation in C++ ==========" << std::endl;

    // EXAMPLE: Stack Memory based Object Instantiation
    transmitData();

    // EXAMPLE: Heap Memory based Object Instantiation
    UartDriver* myGlobalDriver = createPersistentDriver();
    
    // Use the driver for the entire lifetime of the program
    for(int i=0; i<10; i++) {
        myGlobalDriver->send();
    }

    // Manual Destruction is REQUIRED
    // if you forget this line, you have a Memory leak.
    // This frees the memory and runs the Destructor.
    delete myGlobalDriver;
    
    std::cout << "========================================================" << std::endl;
    std::cin.get();
}