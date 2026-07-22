// Topic: LValues & RValues
#include <iostream>
#include <string>

void processValue(int& value){
    std::cout << value << std::endl;
}

void printValue(const int& value){
    std::cout << value << std::endl;
}

void processTemp(int&& value){
    std::cout << value << "*C" << std::endl;
}

// EXAMPLE: FUNCTION OVERLOADING
// VERSION 1: LValue Reference Overload
// The user passed us a persistent string (like a variable they plan to use again).
// We MUST perform a slow, Deep Copy to respect their ownership.
void storeData(const std::string& text) {
    std::cout << "[LVALUE DETECTED] Doing a slow deep copy of: " << text << "\n";
    // ... allocate memory, run memcpy, etc.
}

// VERSION 2: RValue Reference Overload
// The user passed us a temporary string (like "Immediate Log").
// We know this string is going to be destroyed the millisecond this function ends.
void storeData(std::string&& text) {
    std::cout << "[RVALUE DETECTED] Stealing the heap pointer of: " << text << "\n";
    // ... we don't copy the bytes! We just rip the pointer out of 'text' 
    // and claim the memory for ourselves!
}


int main(){
    std::cout << "========== TOPIC: LValues & RValues in C++ ==========\n";

    // EXAMPLE: BASIC
    int i = 10;
    // 'i' is an LValue (It has a memory address on the stack)
    // '10' is an RValue (It is just a temporary literal value).

    //10 = i; // COMPILER ERROR: You cannot assign to an RValue!

    int a = i; // OK: Assigning an LValue to an LValue
    int b = (i + 5);
    // (i + 5) is an RValue. It calculates '15' in the CPU register
    // and assigns it to 'b', and then the '15' instantly ceases to exist.

    // EXAMPLE: LValue References (&)
    int x = 50;
    processValue(x);    // SUCCESS: 'x' is an LValue.
    // processValue(50);   // ERROR: Cannot Bind a non-const LValue reference to an RValue!

    // EXAMPLE: const LValue References
    printValue(x);      // OK: LValue
    printValue(150);    // OK: RValue (Complier secretly makes a temp variable for '50')

    // EXAMPLE: RValue References (&&)
    int xm = 10;
    // processTemp(xm);    // ERROR: 'xm' is an LValue.
    
    processTemp(17);        // SUCCESS: '17' is an RValue
    processTemp(xm + 17);   // SUCCESS: 'xm + 17' i.e. 27 is an RValue (assume xm is 10)

    // EXAMPLE: Function Overloading
    std::string myLog = "Sensor Read";
    
    // The compiler sees 'myLog' is an LValue. It calls Version 1.
    storeData(myLog); 
    
    // The compiler sees a raw literal being converted to a temporary string. 
    // It is an RValue! It calls Version 2 for maximum speed!
    storeData("Direct Warning"); 

    std::cout << "=====================================================\n";
    std::cin.get();
}