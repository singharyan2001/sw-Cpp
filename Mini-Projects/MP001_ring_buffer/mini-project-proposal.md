# Mini-Project 1: Procedural Ring Buffer (FIFO)

## Project Overview
A Ring Buffer (Circular Queue) is a fixed-size buffer that wraps around to the beginning when it reaches the end. It is the backbone of UART/SPI communication stacks and DMA transfers in firmware. This project serves as a "Bridge" to transition your C-style procedural thinking into C++ syntax by enforcing the use of C++ References in place of raw C pointers.

## Core Objectives
1. **Memory Mechanics:** Master modulo arithmetic (`%`) for safe, wrap-around array indexing.
2. **C++ Syntax:** Completely replace C-style pointer arguments (`*`) with C++ Pass-by-Reference (`&`) for state manipulation.
3. **Standard I/O:** Utilize `<iostream>` for runtime debugging and state verification.

## Technical Requirements
1. **Language:** C++17 (Procedural only. No class keywords).
2. **Build System:** CMake (Integrated into the Master Workspace as MP001_ring_buffer).
3. **Memory:** Fixed-size static array defined by a #define or constexpr.

## Functional Specifications (The API)
You must define a `RingBuffer` struct and the following standalone functions to manipulate it. Every function that modifies or reads the buffer must take the RingBuffer struct as a C++ Reference (`RingBuffer& rb`).
- struct `RingBuffer`
    - Contains: An array of integers (e.g., `int data[10]`), a head index, a tail index, and a count (to track current capacity).
- `void initBuffer(RingBuffer& rb)`
    - Resets head, tail, and count to 0.
- `bool push(RingBuffer& rb, int value)`
    - Inserts data at the head index. Advances the head.
    - Rule: Must fail gracefully and return false if the buffer is full.
- `bool pop(RingBuffer& rb, int& out_value)`
    - Extracts data from the tail index. Advances the tail.
    - Rule: Uses a reference out_value to return the data, freeing up the actual return statement to output a boolean success/fail flag.
- `void printStatus(const RingBuffer& rb)`
    - Prints the current capacity, head, and tail locations.
    - Rule: Must use a const reference to guarantee the print function doesn't accidentally alter the state.

## Testing & Validation (In main.cpp)
Write a sequence in `main()` that proves your logic works:
1. Push 5 values into the buffer.
2. Pop 2 values out.
3. Push enough values to intentionally overflow the buffer and verify your push() function rejects the overflow safely.
4. Pop all values until the buffer is empty and verify pop() handles the underflow safely.

---