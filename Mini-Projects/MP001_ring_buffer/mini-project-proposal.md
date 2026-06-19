# Mini-Project 1: Procedural Ring Buffer (FIFO)

## Project Overview
A Ring Buffer (Circular Queue) is a fixed-size buffer that wraps around to the beginning when it reaches the end. It is the backbone of UART/SPI communication stacks and DMA transfers in firmware. This project serves as a "Bridge" to transition your C-style procedural thinking into C++ syntax by enforcing the use of C++ References in place of raw C pointers.

### Core Objectives
1. **Memory Mechanics:** Master modulo arithmetic (`%`) for safe, wrap-around array indexing.
2. **C++ Syntax:** Completely replace C-style pointer arguments (`*`) with C++ Pass-by-Reference (`&`) for state manipulation.
3. **Standard I/O:** Utilize `<iostream>` for runtime debugging and state verification.

### Technical Requirements
1. **Language:** C++17 (Procedural only. No class keywords).
2. **Build System:** CMake (Integrated into the Master Workspace as MP001_ring_buffer).
3. **Memory:** Fixed-size static array defined by a #define or constexpr.

### Functional Specifications (The API)
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

### Testing & Validation (In main.cpp)
Write a sequence in `main()` that proves your logic works:
1. Push 5 values into the buffer.
2. Pop 2 values out.
3. Push enough values to intentionally overflow the buffer and verify your push() function rejects the overflow safely.
4. Pop all values until the buffer is empty and verify pop() handles the underflow safely.

---

## Project Design

### RingBuffer Handle Design
The Ring buffer structure would need to keep track of information of the buffer and its mechanisms i.e. buffer address, max buffer length, buffer status, head index or write pointer, tail or erad pointer, byte_count or bytes_written, and buffer overflow flag.

The Ring buffer would also require an enum for return functions to understand whether the buffer operation was OK, IN-ERROR, OVERFLOW, FULL, and EMPTY.

```cpp
// Ring Buffer Enums
typedef enum{
    RBUF_OK,
    RBUF_EMPTY,
    RBUF_FULL,
    RBUF_OVERFLOW,
    RBUF_IN_ERROR
} rbuf_status_t;

// Ring Buffer Structure Handle
typedef struct{
    uint8_t* rbuf_addr;
    uint8_t rbuf_max_length;
    uint8_t head_index;
    uint8_t tail_index;
    uint8_t bytes_written,
    bool rbuf_overflow;
} rbuf_handle_t;
```

### Ring Buffer Design Consideration
1. EMPTY Condition >> This Condition can be detected by the relation between head index and tail index. When `head index == tail index`, we can consider the ring buffer to be empty.
2. FULL Condition >> This Condition can be detected by keeping a counter to see how many bytes are written i.e. whenever bytes are pushed to the buffer, the count value can be incremented by 1 and whenever bytes are read from the ring buffer, the count can be decremented by 1. Therefore the buffer is full when `bytes_written_count == max_buffer_length`.
3. Role of Modulus Operator >> The Modulus operator can be used to get the remainder of the operation, e.g. head index is 9, tail index is 0, and max length is 10, so we can consider there are 10 (0-9) bytes in the buffer, so we can do `(head + 1) % buffer size` i.e. (10+1) % 10 = 10/10 -> 0 remainder i.e. the index would wrap around from 9 to 0. Therefore achieving the wrap around mechanism.

---

## Mistakes done in mini-project implementation

### C Style Implementation Habit in C++
Note: This was not an issue wrt compilation and execution, but C Style habit was implemented for enums an structures, just implemente using C++ style
1. using typedef for enum and structure - which is not required in c++ as 
2. Used #define to creatq quick printf functions which is C Style, in Mordern C++ >> macros are heavily discouraged because they bypass the compiler's type-checking and make debugging difficult, move from `macros` to `inline functions` as they give exact same performance as a macro but are completely type-safe and respect namespaces.

### Mistakes done in the mini-project
1. Forgot to use the modulo operation during push and pop operations, immediately integrated and tested.
2. Using namespace std in a header file -> should not be used as it will create problems in the global space >> in `log.h`, directly use `std::cout`.
3. Fixed a bug wrt buffer addres print output.
