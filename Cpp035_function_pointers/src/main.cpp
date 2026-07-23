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

int main(){
    std::cout << "========== TOPIC: Function Pointers in C++ ==========\n";
    
    /* EXAMPLE: Basic Understanding of Pointer to a Function*/
    // This is basically &HelloWorld (no need for & because the compiler performs an Implicit Conversion)
    auto function = HelloWorld;
    // Test
    function();
    function();
    std::cout << std::endl;

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

    std::cout << "=====================================================\n";
}