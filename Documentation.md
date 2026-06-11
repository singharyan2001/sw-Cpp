# C++ Documentation

## Overview
C++ is fundamentally an extension of C that introduces object-oriented and generic programming paradigms. Its core philosophy is providing zero-cost abstractions—meaning you can write highly structured, readable code without sacrificing the execution speed, memory layout control, or bare-metal hardware access you would expect from C.

For systems development, C++ shifts the focus from managing raw data and functions to modeling state, owning resources, and enforcing safety at compile time.

## Roadmap

### Phase 1: The Foundations & Modularity
```
Note: This Phase Covers the Chapters 1-3 of the reference book "A Tour of C++"
```
The goal here is to stop writing C and start utilizing C++'s strong type system and organizational features. The book moves fast through pointers, references, and basic classes.
- Focus Topics:
    - References vs. Pointers: Master passing by reference (&).
    - constexpr: Moving calculations to compile-time.
    - User-Defined Types: struct, class, and enum class (strongly typed enums).
    - Modularity: Namespaces and basic exception handling.
- Actionable Implementation: Create a clean hardware abstraction header.
    - Define a namespace (e.g., namespace hw::uart).
    - Use enum class to define strict states for your peripherals (e.g., BaudRate, PinState).
    - Write a basic struct to represent a hardware configuration, using references instead of pointers for initialization.

### Phase 2: Lifetimes and Abstraction
``` 
Note: This Phase Covers the Chapters 4-6 of the reference book "A Tour of C++"
```
This is where the paradigm shift happens. You will learn how C++ manages memory and resources automatically.
- Focus Topics:
    - RAII (Resource Acquisition Is Initialization): The single most important concept in the book. You will learn constructors and destructors.
    - Class Hierarchies: Abstract classes (interfaces) and virtual functions.
    - Move Semantics: Transferring ownership of data safely without copying (std::move).
    - Templates: Writing generic functions and classes.
- Actionable Implementation: Build a C++ wrapper for a C-based system library (like a serial port or GPIO interface).
    - Create an abstract base class called Peripheral.
    - Write a concrete class (e.g., UartDriver) that acquires the file descriptor or hardware lock in the constructor and releases it in the destructor (RAII).
    - Ensure that if an error occurs during initialization, the class throws an exception rather than returning an integer error code.

### Phase 3: The Standard Library & Memory
```
Note: This Phase Covers the Chapters 7-13 of the reference book "A Tour of C++"
```
The book shifts from the core language to the Standard Template Library (STL). You will learn to rely on the STL instead of writing custom linked lists or unsafe raw arrays.
- Focus Topics:
    - Containers: `std::array` (fixed size, perfect for bare-metal/stack allocation) and `std::vector` (dynamic buffers).
    - Smart Pointers: `std::unique_ptr` and `std::shared_ptr`. You should practically never use new and delete manually again.
    - Algorithms: Using <algorithm> for data manipulation instead of writing raw for loops.
- Actionable Implementation: Develop the payload handler for a protocol stack.
    - Use `std::vector<uint8_t>` to manage incoming data streams.
    - Use `std::unique_ptr` to manage the lifecycle of a protocol node—ensuring that only one part of the system can "own" the active master node at a time.
    - Use `std::find` or `std::copy` from the algorithms library to parse and extract frames from a data stream.

### Phase 4: Concurrency & Systems Integration (Chapters 14–15)
```
Note: This Phase Covers the Chapters 14-15 of the reference book "A Tour of C++"
```
Modern systems require asynchronous operations, especially when handling high-speed communication or bridging hardware interrupts to software processing.
- Focus Topics:
    - Threads and Tasks: <thread> and std::async.
    - Synchronization: <mutex>, std::lock_guard, and <atomic>.
- Actionable Implementation: Implement a thread-safe message queue.
    - Create a system where one thread continuously polls a hardware interface (like an RS485 bus) and pushes parsed messages into a queue.
    - Protect that queue using a std::mutex and a std::lock_guard (another form of RAII) so a separate processing thread can safely read the messages without data corruption.

### Strategy for Reading the Tour
1. Iterative Builds: Pair the book with modern, target-based CMake. Create a new add_executable or add_library target for each chapter's concept. This builds muscle memory for both the language and the build system.
2. Type it Out: Stroustrup provides short, brilliant code snippets. Do not just read them. Type them out, compile them, and intentionally break them to see what errors the compiler throws.
3. Embrace the "Why": The book heavily emphasizes why a feature exists (e.g., why std::unique_ptr is safer than a raw pointer). Internalizing the "why" is what separates C coders from C++ architects.

```
Note
1. In the book, Chapter 1 is literally called "The Basics." This is where you learn the syntax and mechanics that separate C++ from C or other languages.
2. Stroustrup doesn't use the buzzword "OOPs" as a chapter title; instead, he breaks it down into "Classes" and "Class Hierarchies." This is where you learn the four pillars of OOP (Encapsulation, Abstraction, Inheritance, Polymorphism) applied to C++.
3. Chapter 4 will cover Encapsulation, Chapter 4 & 5 will cover Abstraction & Inheritance, and Polymorphism post mid chapter 5 and 6.
```

### Strategy of learning C++
```
Note:
We will be using two resources:
1. The Video Resource: The Cherno (Youtube Playlist)
2. The Text Resource: A Tour of C++
```

**The Dual-Engine Learning Strategy**
To learn this effectively without hitting a wall, synchronize them by topic rather than trying to finish one before starting the other.
1. Watch the Concept: Watch The Cherno's videos on a topic (e.g., Classes, Destructors, or Smart Pointers) to get the visual and low-level layout breakdown.
2. Read the Standard: Read the brief corresponding sections in A Tour of C++ to see the official, modern syntax and architectural guidelines.
3. Build the Driver: Write a small piece of mock firmware code in your build system. For example, if you just learned about destructors and RAII, write a class that simulates opening, reading from, and safely closing an SPI or UART device node.

--- ---

## Cpp YT Playlist - The Cherno

### How C++ Works

```text
Lecture 01 - how C++ Works
Notes:
1. Preprocessors in C++
2. iostream in C++
3. << in C++ and when its used with cout
4. C++ Compilation Workflow
5. Debug vs Release, platforms like x84 & x64, solution configuration s for C++
6. Header files and source files
7. Declaration  and Definition in C++
8. Linker in C++ 
```

#### Preprocessors in C++
- The preprocessor is a text-manipulation tool that runs before the actual compiler even sees your code.
- Any line starting with `#` e.g. `#include` or `#define` is a preprocessor directive.
- When you write `#include "dock_hw.h"`, the preprocessor physically opens that header file, copies all the text inside it, and pastes it into your `.cpp` file.

#### iostream in C++
- This is the standard library file that handles input and output streams (like priniting to a console).
- It contains the declarations for objects like `std::cout` and `std::cin`.
- You have to `#include` it because C++ inherently does not know how to print text to a screen, it relies on this library to interface with the operating system's standard output.

#### `<<` in C++
- In raw C, `<<` is strictly a bitwise left-shift operator used for register manipulation.
- C++ introduces `operator overloading` which allows you to redefine what standard symbols do.
- The iostream library overloads << to act as an "insertion" operator.
- When you write `std::cout << "Data"`, you aren't doing bit math. You are taking the "Data" and pushing it into the cout stream object. It is conceptually identical to calling a method on an object in Python, like `dock.enclosure("Data")`. You are just using an operator symbol instead of a standard function call to pass that data into the object.

#### C++ Compilation Workflow
The C++ build process happens in three distinct, isolated phases:
1. **Preprocessing:** Resolves all `#` directives (copy-pasting text).
2. **Compilation:** The compiler translates each individual `.cpp` file into an intermediate machine-code file called an Object file (`.o` or `.obj`). It does this blindly, file by file, without knowing about the rest of your project.
3. **Linking:** The linker takes all those standalone Object files and stitches them together into the final executable binary.

##### Debug vs Release & Solution Configurations
These are build rules that tell the compiler how to generate your machine code.
1. **Debug:** The compiler leaves the code mostly unoptimized and injects "debug symbols" (markers that map the machine code back to your exact lines of C++). It runs slower, but allows you to step through the code line-by-line.
2. **Release:** The compiler aggressively optimizes the code for maximum speed and minimum footprint, stripping out all debug symbols.
3. **x86 vs x64 (Architecture):** This specifies the target CPU instruction set. x86 is for 32-bit processors, and x64 is for 64-bit processors. If you were cross-compiling this C++ code for your RPi5 later, you would switch this target configuration to ARM64.

#### Declarations and Definitions
1. **Declaration:** You are making a promise to the compiler. `void initDock();` tells the compiler, "A function with this name and return type exists somewhere. Trust me." (This goes in the Header).
2. **Definition:** You are fulfilling the promise. `void initDock() { /* setup code */ }` is the actual memory and logic allocated to that function. (This goes in the Source file).

#### Linker in C++
- Because the compiler works on each `.cpp` file in complete isolation, it relies entirely on Declarations to compile successfully.
- The Linker is the final glue. It searches through all the generated `.o` files to match the Declarations with their actual Definitions.
- If you promised the compiler that `initDock()` existed, but you forgot to write the definition in your `.cpp` file (or forgot to tell CMake to compile that specific file), the compiler will pass, but the Linker will fail and throw an "unresolved external symbol" error.

