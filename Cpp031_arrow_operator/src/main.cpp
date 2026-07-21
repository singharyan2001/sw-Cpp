// Topic: Arrow Operator
#include <iostream>
#include <cstddef> // For offsetof macro

// EXAMPLE 1
struct SensorData {
    int temperature;
    int pressure;

    SensorData(int temp, int pressure) : temperature(temp), pressure(pressure) {};

    void print(){
        std::cout << "Temp: " << temperature << ", Pressure: " << pressure << std::endl;
    }
};

// EXAMPLE 2
class UartDriver {
public:
    void sendData(){
        std::cout << "Sending UART data...\n";
    }
};

// A custom, simplified smart pointer
class ScopedUart {
private:
    UartDriver* m_RawPointer;
public:
    ScopedUart(UartDriver* ptr) : m_RawPointer(ptr) {};
    
    ~ScopedUart() { 
        delete m_RawPointer;
    }

    // Overloading the Arrow Operator
    // We return the raw pointer, allowing the user to call UartDriver methods directly!
    UartDriver* operator->(){
        return m_RawPointer;
    }
};

// EXAMPLE 3
struct Packet{
    char id;        // 1 byte
                    // 3bytes padding injected by the compiler
    int payload;    // 4 bytes
    float voltage;  // 4 bytes
};


int main(){
    std::cout << "========== TOPIC: ARROW OPERATOR ==========\n";
    
    //===================== BASIC EXAMPLE: SYNTAX =============================//
    // 1. Normal Object (Use Dot)
    SensorData localSensor = {25, 1013};
    localSensor.print();

    // 2. Pointer to Object
    SensorData* ptr = &localSensor;

    // The Clunky way (Dereference, then dot):
    (*ptr).temperature = 26;

    // The Clean way (The Arrow Operator)
    ptr->pressure = 1010;
    ptr->print();

    //===================== ADVANCED EXAMPLE: OVERLOADING THE ARROW OPERATOR =============================//
    // We create a wrapper object on the stack
    ScopedUart myUart(new UartDriver());

    // Because we overloaded ->, this perfectly forwards the call to the raw UartDriver pointer!
    myUart->sendData();

    // ~ScopedUart() automatically deletes the raw pointer here.

    //==================== EXAMPLE: Memory Offset Trick =================================================//
    // 1. We cast the literal number '0' to a Packet pointer
    // DANGER: We cannot read or write to this pointer (it would crash)
    // but we can ask the compiler for the ADDRESS it would calculate

    // We ask: "If a Packet started at address 0, what address would voltage be at?"
    // int offset = (long)&((Packet*)0)->voltage;
    int offset = (long)&((Packet*)nullptr)->voltage;

    std::cout << "Manual Offset of voltage: " << offset << " bytes\n";
    // Output is typically 8 bytes (1 for char + 3 padding + 4 for int).

    // 2. The Standard C++ Way
    // Modern C++ provides a macro that does exactly this under the hood safely:
    std::cout << "Macro Offset of voltage: " << offsetof(Packet, voltage) << " bytes\n";

    std::cout << "===========================================\n";
    std::cin.get();
}