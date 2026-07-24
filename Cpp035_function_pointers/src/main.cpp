// Topic: Function pointers in C++
#include <iostream>
#include <vector>

void HelloWorld(){
    std::cout << "Hello World!" << std::endl;
}

void PrintTemp(float value){
    std::cout << "Temp Value: " << value << std::endl;
}

void PrintValue(int value){
    std::cout << "Value: " << value << std::endl;
}

void ForEach(const std::vector<int>& values, void(*func)(int)){
    for(int value : values){
        func(value);
    }
}

void PrintSystemStatus(){
    std::cout << "SYSTEM STATUS: OPERATIONAL" << std::endl;
}

// Using Alias for function pointers
using SystemOut = void(*)();

// Passing Functions as Arguments (The STL Way)
// A Generic Processing function that takes a vector AND a function pointer
void ProcessArray(const std::vector<int>& data, void(*action)(int)){
    for(int value : data){
        action(value); // Execute the injected logic
    }
}

// Our Custom Actions
void printStandard(int v){
    std::cout << v << " ";
}

void printHex(int v){
    std::cout << std::hex << "0x" << v << std::dec << " ";
}

// Embedded Use Case: Hardware Interrupt Callback
// ==========================================
// LOW-LEVEL HAL (Hardware Abstraction Layer)
// ==========================================
using InterruptCallback = void(*)();    // Clean alias for out callback

class GpioDriver {
private:
    InterruptCallback m_userCallback = nullptr;
public:
    //1. The API to register a function pointer
    void attachInterrupt(InterruptCallback callback) {
        m_userCallback = callback;
        std::cout << "[HAL] Callback registered successfully.\n";
    }

    // 2. Simulated Hardware Trigger
    void simulateVoltageSpike() {
        std::cout << "[HARDWARE] Voltage spike detected on pin!\n";
        
        // If a callback is registered, jump to that memory address and execute it!
        if (m_userCallback != nullptr) {
            m_userCallback(); 
        }
    }
};

// ==========================================
// HIGH-LEVEL APPLICATION
// ==========================================

// Our custom logic we want to run when the button is pressed
void emergencyStop() {
    std::cout << "[APP] EMERGENCY STOP TRIGGERED! Cutting motor power.\n";
}

int main(){
    std::cout << "========== TOPIC: Function Pointers in C++ ==========\n";
    
    /* EXAMPLE: Basic Understanding of Pointer to a Function*/
    // This is basically &HelloWorld (no need for & because the compiler performs an Implicit Conversion)
    auto function = HelloWorld;
    // Test
    function();
    function();
    std::cout << std::endl;

    // Using alias for function pointers
    SystemOut DiagnosticEngine = PrintSystemStatus;

    DiagnosticEngine();

    // Normal Function: type + function name + parenthesis which holds the possible input arguments
    // Function Pointer: type + *function name + paraenthesis (input arguments if needed)
    void(*function1)();
    function1 = HelloWorld;
    // Test
    function1();
    function1();
    std::cout << std::endl;

    // We can use auto or typedef or using to create a function pointer
    typedef void(*HelloWorldFunction)();

    HelloWorldFunction myFunctionPtr = HelloWorld;

    myFunctionPtr();
    myFunctionPtr();
    std::cout << std::endl;

    // Function Pointer with input arguments
    typedef void(*PrintFunction)(float value);

    PrintFunction myPrintPtr = PrintTemp;
    myPrintPtr(24.7);
    myPrintPtr(25.5);
    std::cout << std::endl;

    // EXAMPLE: Using Function Pointer in an application like with vectors
    std::vector<int> values = {1,5,4,2,3};
    ForEach(values, PrintValue);

    // Passing function pointers as arguments
    std::vector<int> payload = {10, 15, 20, 255};
    std::cout << "Standard: ";
    ProcessArray(payload, printStandard);

    std::cout << "\nHex: ";
    ProcessArray(payload, printHex);
    std::cout << std::endl;

    // Embedded Use Case: Hardware Interrupt Callbacks
    GpioDriver estopPin;

    // We pass the memory address of 'emergencyStop' to the hardware driver
    estopPin.attachInterrupt(emergencyStop);

    std::cout << "Drone is flying normally...\n";

    // User triggers the ESTOP Button
    estopPin.simulateVoltageSpike();

    std::cout << "=====================================================\n";
}