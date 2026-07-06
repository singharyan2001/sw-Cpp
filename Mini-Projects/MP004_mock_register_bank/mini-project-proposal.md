# Mini-Project 4: Mock Register Bank

## Project Overview
In Firmware, you often need to read.write specific bits in hardware registers without accidentlly corruptiong adjacent memory. This project challenges you to build a Concrete Type (`RegisterBank`) that overloads C++ operators so you can interact with a simulated memory map as if it were a simple standard array.

## Core Objectives
1. **Concrete Types:** Build a self-contained class that acts like a primitive type.
2. **Operator Overloading:** Implement the [] operator for array-style access and the << operator for clean debugging.
3. **Const Correctness:** Enforce read-only guarantees on specific methods.

## Technical Requirements
1. Language: C++17.
2. Build System: CMake (Integrated into the Master Workspace as 04_register_bank).
3. Memory: Simulate a bank of four 32-bit registers using an internal uint32_t array.

## Functional Specifications

Create a class named `RegisterBank`.
- Part A: Internal State & Construction
    - Private Data: `uint32_t registers[4];`
    - Constructor: Initialize all registers to `0x00000000` using a Member Initializer List or inside the `{}` body.
- Part B: Operator Overloading (`[]`)
    - You need to allow the user to read and write to the registers using brackets (e.g., `myBank[2] = 0xFF;`). To do this, you must overload the subscript operator.
    - Write Access: Overload `operator[]` to return a `uint32_t&` (reference) so the user can assign a value to it.
    - Read-Only Access: Overload `operator[]` a second time as a const method returning `const uint32_t&`. This allows you to read registers even if the RegisterBank object is passed into a function as a const reference.
- Part C: Global Operator Overloading (`<<`)
    - Overload the `<<` operator for `std::ostream` so you can pass the entire `RegisterBank` object directly to `std::cout`.
    - Format: Have it print out the hex values of all 4 registers cleanly.

## Testing & Validation (In main.cpp)
Your `main.cpp` must prove your class behaves like a primitive concrete type:
1. Instantiate a `RegisterBank` on the stack.
2. Use the overloaded `[] operator` to write `0xDEADBEEF` to index 1.
3. Use the overloaded `[] operator` to write `0xCAFEBABE` to index 3.
4. Attempt to write to index 5 and verify your bounds checking catches the error without crashing.
5. Print the entire register bank to the console using `std::cout << myBank << std::endl;`.

---