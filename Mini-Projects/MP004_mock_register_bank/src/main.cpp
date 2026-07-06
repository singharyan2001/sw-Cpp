// MINI-PROJECT 004: MOCK REGISTER BANK
#include <iostream>
#include <stdint.h>

#include "RegisterBank.h"


int main(){
    std::cout << "========== MINI-PROJECT: MOCK REGISTER BANK ==========" << std::endl;

    RegisterBank memory;
    std::cout << memory << std::endl;

    memory[0] = 0x45;
    memory[1] = 0x1234;
    memory[3] = 0xDEEF;

    std::cout << memory << std::endl;

    // Test 01: Instantiate a RegisterBank on the stack.
    RegisterBank new_memory;

    // Test 02: Use the overloaded [] operator to write 0xDEADBEEF to index 1.
    new_memory[1] = 0xDEADBEEF;

    // Test 03: Use the overloaded [] operator to write 0xCAFEBABE to index 3.
    new_memory[3] = 0xCAFEBABE;

    // Test 04: Attempt to write to index 5 and verify your bounds checking catches the error without crashing.
    new_memory[5] = 0x37;

    // Test 05: Print the entire register bank to the console
    std::cout << new_memory << std::endl;

    std::cout << "======================================================" << std::endl;
    std::cin.get();
}