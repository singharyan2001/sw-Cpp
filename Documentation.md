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

Playlist Link >> https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb


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

## How C++ Compiler Works


## How C++ linker Works


## Variables in C++
- Discussed about variables and memory relation
- Discussed primitive data types and memory consumption of these data types like bool, char, char, short, int, long, long long, float, and double
- Discussed on signed and unsigned data types and how signed data bytes use the 32th bit for representing +/- number.
- Discussed difference between float and double.
- Discussed on `sizeof()` operator.

## Functions in C++
- Discussed on functions - overview, input and output, function types via data types.
- Discussed on how functions being called in runtime.

## Operators in C++ (Reference Book - Ch1:1.5)
- The Arithmetic Operators can be used for appropriate combinations of these types:
```cpp
x+y     // plus
+x      // unar y plus
x−y     // minus
−x      // unar y minus
x∗y     // multiply
x/y     // divide
x%y     // remainder (modulus) for integers
```

- Comparison Operators:
```cpp
x==y    // Equality
x!=y    // Not Equal
x<y     // less than
x>y     // greater than
x<=y    // less than or equal
x>=y    // greater than or equal
```

- Logical Operators
    - The logical operators `&&` and `||` simply return `true` or `false` depending on the values of their operands.
    - A bitwise logical operator yield a result of their operand type for which the operation has been per-formed on each bit.

    ```cpp
    x&y     // bitwise and
    x|y     // bitwise or
    xˆy     // bitwise exclusive or
    ˜x      // bitwise complement
    x&&y    // logical and
    x||y    // logical or
    ```
- Note: that `=` is the assignment operator and `==` tests equality.

## Scope & lifetime in C++ (Reference Book - Ch1:1.6)
- **Local Scope:** A name declared in a function is called a local name, and its scope extends from its point of declaration to the end of the block in which its declaration occurs/
- **Note:** A Block is delimited by a `{ }` pair.
- **Class Scope:** A name is called a member name, if it is definied in a class, outside any function, or enum class, and its socpe extends from the opening `{` of its enclosing declaration to the end of that declaration.
- **Namespace Scope:** A name is called a namespace member name if it is defined in an namespace outside any function, class, or enum class. and its scope extends from the point of declaration to the end of its namespace.
- A name not declared inside any other construct is called a global name and is said to be in the global namespace.
- **Note:** We can have objects without names, such as `temporaries` and objects created using `new`.
- An **object** must be constructed (initialized) before it is used and will be destroyed at the end of its scope.
- For a **namespace object** the point of destruction is the end of the program.
- For a **member**, the point of destruction is determined by the point of destruction of the object of which it is a member.
- An Object created by `new` "lives" until destroyed by `delete`.

## Constants in C++ (Reference Book - Ch1:1.7)
- C++ supports two notions of immutability i.e. `const` and `constexpr`.
- `const` means roughly "I promise not to change this value", this is used primarily to specify interfaces, so that data can be passed to functions without fear of it being modified and the compiler enforces the promise made by const.
- `constexpr` means roughly "to be evaluated at compile time", and this is used primarily to specify constants, to allow placement of data in read only memory (where ir is unlikely to be corrupted) and for performance.

```cpp
const int dmv = 17;         // dmv is a named constant
int var = 17;               // var is not a constant

constexpr double max1 = 1.4∗square(dmv);    // OK if square(17) is a constant expression
constexpr double max2 = 1.4∗square(var);    // error: var is not a constant expression
const double max3 = 1.4∗square(var);        // OK, may be evaluated at run time

double sum(const vector<double>&);          // sum will not modify its argument (§1.8)
vector<double> v {1.2, 3.4, 4.5};           // v is not a constant
const double s1 = sum(v);                   // OK: evaluated at run time
constexpr double s2 = sum(v);               // error: sum(v) not constant expression
```

- For a function to be usable in a constant expression i.e. in an expression that will be evaluated by the compiler, it must be defined `constexpr`. for example
```cpp
constexpr double square(double x){
    return x*x
}
```
- To be `constexpr`, a function must be rather simple like return-statement computing a value.
- A `constexpr` function can be used for non-constant arguments, but when that is done the result is not a constant expression.
- We allow a constexpr function to be called with non-constant-expression arguments in contexts that do not require constant expressions, so that we don’t have to deﬁne essentially the same function twice: once for constant expressions and once for variables.
- In a few places, constant expressions are required by language rules e.g. array bounds, case labels, template value arguments, and constants declared using `constexpr`.
- In other cases, compile-time evaluation is important for performance.
- Independently of performance issues, the notion of immutability (of an object with an unchangeable state) is an important design concern.

### Personal Notes on this topic
- In the raw `C` you write for hardware, you usually just `#define` a macro or use a basic `const` and call it a day. But in `C++`, the difference between `const` and `constexpr` comes down to a critical concept for system performance: When is the value figured out?

**`constexpr` - The Compile-Time Calculator (Flash / ROM)**

Think of constexpr as a command to the compiler that says: "Figure out this exact value on my laptop while you are compiling the code, and burn the final number directly into the read-only memory (ROM/Flash)."
- **The Rule:** The compiler must have 100% of the information it needs to do the math before the program ever runs.
- **The Firmware Advantage:** It costs zero CPU cycles at runtime.
- **Example explained from above notes:** `constexpr double max1 = 1.4 * square(17);` Because 17 is a hardcoded number, the C++ compiler on your laptop calculates the square, multiplies it by 1.4, and just embeds the final result into the executable.

**`const` - The Run-Time Guardrail (RAM)**

Think of const as a strict rule for your interfaces: "You can figure out this value while the system is actively running, but once it is set, no one is allowed to change it."
- **The Rule:** The value might depend on something that happens while the program is executing (like a sensor reading or a dynamically changing variable).
- **The Firmware Advantage:** It protects your memory. If you pass a large data packet into a communication stack, labeling it const ensures the protocol layer can read it, but the compiler will throw an error if the protocol layer accidentally tries to overwrite your data.
- **Example explained from above notes:** `const double max3 = 1.4 * square(var);` Because var is a standard variable, its value might change. The compiler cannot know what var is beforehand. So, your hardware has to spend CPU cycles to calculate this math during runtime, store it in RAM, and put a const lock on it so it cannot be altered afterward.

**The `constexpr` Function: A Dual-Purpose Tool**

constexpr function can take non-constant arguments. This is a brilliant feature of modern C++ designed to stop you from writing the same function twice.
- If you write a simple function like this:
```cpp
constexpr double square(doubel x){
    return x*x;
}
```
C++ makes this function "smart" depending on how you use it:
- **Scenario A (Laptop does the work):** If you call it with a hardcoded number: `square(10)`, the compiler calculates 100 during the build process and flashes the constant 100 to the board.
- **Scenario B (Board does the work):** If you call it with a live variable: `square(sensor_reading)`, the compiler says, "I can't calculate that right now," strips away the constexpr promise, and just compiles it as a normal run-time function that your processor will execute.

Therefore:
- Use `constexpr` for hardware constants, pin definitions, and math you want the compiler to do in advance to save CPU Cycles.
- Use `const` for function arguments to protect your structs and variables from accidetal modifications while the system is running.

## C++ Header Files
- Discussed what header files are and why we need them and what it solves.
- Discussed the use `#pragma once` and why its needed and used.
- Discussed the alternative via `#ifndef` based header guard.
- Discussed about #include statements using Double Quotes(`""`) and Angular brackers (`<>`). and also established a rule like angual brackets(`<>`) for compiler included paths and Double Quotes(`""`) for the rest.

## Conditions and Branches in C++
- Discussion on if, if-else, if-else if-else statements.
- Disassembly view of conditional based C++ code

## Loops in C++
- Discussed loops application in C++ (game dev example)
- Discussed for loops, while loops, do-while loops.
- Discussed when to use different kind of loops in C++.

## Control Flow in C++
- Discussed keywords - Continue, break, and return
- Discussed use case / application of the above keywords in loops and functions.

## Pointers in C++
- Discussed on Memory and raw pointers.
- Pointers are important for managing and manipulating memory.
- A Pointer is an integer or number which stores a memory address.
- Discussed void pointers, NULL and nullptr (Cpp11)
- Discussed Asterisk(*) and Ampersand(&) operator and its use-cases in pointers.
- Discussed read & write operations with pointers and also discussed the use of data type when a pointer is declared.
- Talked about stack vs heap memory allocation when it comes to variables and pointers.
- Showed examples of using new and delete keyword, and memset api.
- Discussed double pointers with an example.

## References in C++
- Explained the use `<data-type>*` and `<data-type>&` for pointer declaration and reference declarations.
- Discussed that reference is just an alias, it just exists in the source code and it does not exists in or consume memory when we compare with pointers.
    - Conceptually yes, it is just an alias.
    - If you write `int&b = a;` inside a single function, the compiler usually optimizes it away completely using zero memory.
    - The Exception is when you pass a reference into a function (for e.g. `void process_data(int& data)`), the compiler cannot magically alias accross different stack frames.
    - Under the hood, the compiler implements references as standard pointers, it passes the memory address over the stack exactly like a pointer would, meaning it takes the same bytes of overhead as a pointer.
    - The Benefit is purely syntactic sugar and safet for the programmer.
- Discussed how passing by value, pointer, and reference work with functions.
    ![Call by Value](doc-imgs/Cpp_pass_by_value.png)
    - In Call by Value, a function receives a complete copy of the variable and changes to the parameter inside the function does not affect the original variable in the caller's stack frame.

    ![Call by Pointers](doc-imgs/Cpp_pass_by_pointer.png)
    - In Pass by Pointer, the fucntion receives the memory address (0x2000) of the variable, and the function uses the pointer to reach back and modify the original memory directly.

    ![Call by References](doc-imgs/Cpp_pass_by_reference.png)
    - In Pass by Reference, the function parameter becomes an alias for the original variable, and it shares the same memory address, and behaves like a pointer but with cleaner, 'value like' syntax.

- Discussed how the reference can tied to a variable1, cannot make the same reference point to a new variable called variable2.
    - A pointer can point to something else later, but a reference is locked to its target for life.
- Discussed how pointers and reference can be used together.

## Pointers, Arrays, and References in C++ (Reference Book - Ch1:1.8)
- In Declarations, `[]` means "array of" and `*` means "pointer to".
- All arrays have 0 as their lower bound, so for example char v[6] is from v[0] to v[5].
- The size of an array x[number] must be a constant expression.
- In an Expression, prefix unary `*` means "contents of" and prefix unary `&` means "address of".
- In a Declaration, the unary suffix `&` means "reference to".
- A Reference is similar to a pointer, except that you dont need to use a prefix `*` to access the value referred to by he reference.
- Also, a reference cannot be made to refer to a different object after its initialization.
- References are particular useful for specifying function arguments, for example `void sort(vector<double>& v);`. Therefore by using a reference, we ensure that for a call `sort(my_vec)`, we do not copy `my_vec` and that it is relay `my_vec` that is sorted ad not a copy of it.
- When we don't want to modify an argument, but still don't want the cost of copying, we use a const reference, for e.g. `double sum(const vector<double>&)`

Note: Chapter 01 is now completed in reference textbook - A Tour of C++.

---

## Classes in C++
- Classes group data and functionality together.
- Dicussed on Object and Instances in Classes.
- Discussed on classes, visibility of data in a class, methods (functionality) in classes.

### Notes (reference textbook ch2)
1. A Class is defined to have a set of members, and private members are accessible only through that interface.
2. A function with the same name as its class is called a constructor i.e. a function used to construct objects of a class.
3. There is no fundamental difference between a struct and a class; a struct is simply a class with members public by default.


## Classes VS Structs in C++
- Discussed difference between structs and Class & also on the visibility property.
- Discussed why structs exist in C++ when classes already exists - due to backwards compatibiltiy.
- Discussed application for structs and Classes in C++
- Discussed struct application - to use to represent a structure of data
- Discussed classes applucation - to not only represent data, but for it to also have functionality, visibility, and inheritence concepts applied, would go for classes.
- Note: Structs can also have functions defined in them with data attributes in C++, for e.g.
    ```Cpp
    struct example{
        float x,y;
        void move(example &ex){
            x += ex.x;
            y += ex.y;
        }
    }
    ```

## How to Write a C++ Class
- Discussed Log Class implementation that would contain 3 levels i.e. error, warning, message/trace and set our log systems level to only print to console depending on the level.
- Discussed and implemented the logging class - public variables, public static variables, public methods, private variables and private function implemented.
- Hands on implmentation done on log class implementation

## Static in C++
- Discussed how static has two meanings wrt context
    - static keyword usage outside classes / structs (global level)
    - static keyword usage inside classes / structs (internally in classes/structs)
- Discussed static and extern keywords and usage
- Discussed using static on global variables and with functions
- The Cherno explains the difference between using the static keyword inside and outside of classes. The discussion focuses on how static influences symbol linkage across translation units, affecting variable and function visibility.
- Discussed how static is used in classes wrt attributes and methods.
- Discussed why we need to intialize the static variables inside a class or struct.
- Discussed about static methods.
- Discussed how we can just access static attributes and methids via namespace and why this is how they should be accessed.
- Discussed why static methods cannot access non static attributes of a class, and this can be solved by passing on the class via arguments of that static method of he class.
- Discussed the differences between static and non-static attributes and methods
- Discussed how a c++ class actually works (internals)
- The Cherno demonstrates how static variables and methods function within classes and structs, focusing on memory sharing across instances. Examples highlight the distinction between static and non-static members and their behavior during compilation.


### Personal notes on static
1. In C++, the word `static` is overloaded. It dictates two completely different behaviors depeding entirely on where you type it - Linkage (Visibility) VS Lifetime (Storage)
2. **Context 1:** Outside a class (file scope/Global level)
    1. Internal Linkage basically imples that this symbol is invisible outside this specific `.cpp` file.
    2. By default, any global variable or standalone function we write has External Linkage. When the compiler turns `sensor.cpp` into `sensor.o`, it puts that variable's name into a public "Symbol Table" so the linker can let other `.cpp` files connect to it via `extern`.
    3. Adding `static` acts as a Linker Cloak:
        1. Static changes the linkage from external to internal
        2. The compiler still puts the variable in RAM, but it refuses to write its name into the public symbol table.
        ```cpp
        // 3. Example >> inside driver_a.cpp
        int global_speed = 100; // Public to the whole project
        static int private_offset = 12; // Strictly trapped inside driver_a.cpp file
        ```

    4. Static prevents Namespace pollution, for e.g. if `i2c_driver.cpp` and `spi_driver.cpp` both need a local tracking variable named `static bool is_busy`, marking them both statuc guarantees the linker won't crash with a `Multiple Definition`error, since they exisit as two totally isolated addresses in RAM.
3. **Context 3:** Inside a Local Function (Function Scope)
    1. In C++, using static inside a local function implies "Persistent Lifetime" i.e. intialize me once, remember my value forever.
    2. When you put `static` on a variable inside a function, it does not live on the stack; it lives in the global data segment.
    3. The line of code that intializes it is only executed the  very first time the CPU calls that function.
    ```cpp
    void triggerheartbeat(){
        static uint32_t last-toggle_time = 0; // Evaluated ONCE at boot
        if(millis() - last_toggle_time >= 1000){
            toggleLed();
            last_toggle_time = millis(); //Remembers this value for the next call!!
        }
    }
    ```

## Unions in C++

### Notes (reference textbook ch2)
1. A union is a struct in which all members are allocated at the same address so that the union occupies only as much space as its largest member.
2. Naturally, a union can hold a value for only one member at a time.
3. The language doesn’t keep track of which kind of value is held by a union, so the programmer must do that.

## Enums in C++
- Discussed Enum which are a set of values which the user gives a name to for readability.
- Enum is basically an integer
- You can specify the type of integer you want the enum to be such as `enum Example :: unsigned char`
- By default, enum’s are 32-bit integers
- integrating enum to log class example
- using log::<enum> as a log namespace in main code to access enum
- will later focus on enum classes in C++
- **Video Summary:** The Cherno demonstrates how to use enumerations to name integer values, improving code readability and grouping related constants. Learn to define custom types to restrict valid inputs and keep code cleaner.

### Notes (reference textbook ch2)
1. In C++, there are two ways to work with enummeration i.e. enum and enum class.
    1. **Enum** >> basically an unscoed enum, which leaks its values into the surrounding scope and allows implicit conversions to integers
    2. **Enum Class** >> basically a scoped enum whcih restricts its values to its own scope and strictly prevents implicit integer conversion.
2. In addition to classes, C++ supports a simple form of user-deﬁned type for which we can enumerate the values.
    ```cpp
    enum class Color { red, blue, green };
    enum class Trafﬁc_light{ green, yellow, red };

    Color col = Color::red;
    Trafﬁc_light light = Trafﬁc_light::red;
    ```
3. Enumerations are used to represent small sets of integer values. They are used to make code
more readable and less error-prone than it would have been had the symbolic (and mnemonic) enumerator names not been used.
4. The class after the enum speciﬁes that an enumeration is strongly typed and that its enumerators are scoped.
5. Class & Enums, Being separate types, enum classes help prevent accidental misuses of constants.
6. In particular, we cannot mix Trafﬁc_light and Color values:
    ```cpp
    Color x = red;                  // error : which red?
    Color y = Trafﬁc_light::red;    // error : that red is not a Color
    Color z = Color::red;           // OK
    ```
7. Similarly, we cannot implicitly mix Color and integer values:
    ```cpp
    int i = Color::red;     // error : Color ::red is not an int
    Color c = 2;            // error : 2 is not a Color
    ```
8. The enumerators from a "plain" enum are entered into the same scope as the name of their enum and implicitly converts to their integer value. for e.g.
    ```cpp
    enum Color { red, green, blue };
    int col = green;
    ```
9. The "plain" enums have been in C++ (and C) from the earliest days, so even though they are less well behaved, they are common in current code.

**Notes:**
1. Chapter 2 of reference text book "A Tour of C++" is now completed, moving to chapter 3.
2. Chapter 2 contents - structs, class, unions, and enums covered.

---

## Constructors in C++
- Constructor is a special type of method in classes, which runs whenever instantiated an object.
- You can create a constructor by creating a function with the class name, for e.g. `class entity` → `entity(){ }` and if needed you can also pass arguments like `entity(int a, int b){ }` .
- We can hide the constructor if not needed like a static class by including the entity under private section of the class.
- If the user does not want the inbuilt constructor of a class, so that the object is not instantiated by just writing this piece of code >> `classname() = delete;`
- **Video Summary:** The Cherno demonstrates how to properly initialize class member variables using special methods called upon object instantiation. The tutorial covers creating default and parameterized constructors to replace manual initialization methods.

```cpp
#include <iostream>

class log{
public:
    enum class Log_Level{
        LEVEL_ERROR,
        LEVEL_WARNING,
        LEVEL_INFO
    };
private:
    Log_Level __log_level;
public:
    // Default Constructor
    log(){
        __log_level = Log_Level::LOG_LEVEL_INFO;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << (int)__log_level << std::endl;
    }

    // Constructor with input arguments option
    log(Log_Level level){
        __log_level = level;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << (int)__log_level << std::endl;
    }

    // APIs
    void log_set_level(Log_Level level){
        //
    }
    
    void log_info(const char* message){
        //
    }

    void log_warning(const char* message){
        //
    }

    void log_error(const char* message){
        //
    }
}

int main(){
    log file_logger(log::Log_Level::LEVEL_WARNING);
    log console_logger(log::Log_Level::LEVEL_INFO)
}
```

## Destructors in C++
- Destructors run when you destroy an object
- You can create a destructor in a similar way like its done for a constructor, but just add a tilde (~) as a prefix, for e.g. `~entity(){ }`
- Implemented the entire constructor to destructor cycle of an object by creating an object in a function to show the constructor and destructor outputs via print.
- **Video Summary:** The Cherno demonstrates how to implement destructors to manage memory cleanup when objects go out of scope or are deleted. Examples include using breakpoints in Visual Studio to trace when the destructor is called on stack-allocated objects.

```cpp
#include <iostream>

class log{
public:
    enum class Log_Level{
        LEVEL_ERROR,
        LEVEL_WARNING,
        LEVEL_INFO
    };
private:
    Log_Level __log_level;
public:
    // Default Constructor
    log(){
        __log_level = Log_Level::LOG_LEVEL_INFO;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << (int)__log_level << std::endl;
    }

    // Default Destructor
    ~log(){
        std::cout << "LOG INSTANCE WILL BE DESTROYED DUE TO DESTRUCTOR BEING TRIGGERED" << std::endl;
    }

    // Constructor with input arguments option
    log(Log_Level level){
        __log_level = level;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << (int)__log_level << std::endl;
    }

    // APIs
    void log_set_level(Log_Level level){
        //
    }
    
    void log_info(const char* message){
        //
    }

    void log_warning(const char* message){
        //
    }

    void log_error(const char* message){
        //
    }
}

int main(){
    log file_logger(log::Log_Level::LEVEL_WARNING);
    log console_logger(log::Log_Level::LEVEL_INFO)
}
```

## Inheritance in C++
- Discusses on Inheritance in C++ and how it helps programmers avoid duplication.
- Implemented an example of inheritance, where a base class called `entity` and sub-class `player` was created and new functionality was added in the player class.
- Discussed on the visibility concept wrt base class and sub-classes.
- Discussed an introductary overview on polymorphism and how it can be used between base class and sub-classes.
- Discussed how a programer can create a function that takes in the entity type as its input arguments, and that function is capable of taking the player sub-class via that arguments, because the player class is a subset of the base class entity, therefore it is type player and entity, therefore it is completely valid.
    - Book Definition: A subclass object (Player) is an instance of both itself and its base class (Entity). Because it satisfies an "is-a" relationship (Player is-an Entity), any function expecting a parameter of type Entity will seamlessly accept a Player argument.
- **Video Summary:** The Cherno explores how inheritance allows for a hierarchy of classes, using a base class to share common functionality like position and movement methods with derived subclasses. This approach significantly reduces code duplication while introducing new data members and specialized functionality to specific entities.

```cpp
#include <iostream>
#include <stdint.h>

class i2c_slave{
private:
    uint8_t  m_slave_address;
    uint32_t m_bus_speed;
    bool m_internal_pull_up_en;
public:
    // Default Constructor
    i2c_slave(){
        m_slave_address = 0x40;         // Default Sensor Address
        m_bus_speed = 100000;           //100 Khz - Standard Bus Speed
        m_internal_pull_up_en = true;   // Internal pull-up enable status
        std::cout << "[CONSTRUCTOR] I2C SENSOR INSTANCE INTIALIZED - WITH PARAMS: " << std::endl;
        std::cout << "DEFAULT I2C SENSOR ADDRESS: 0x" << std::hex << (uint)m_slave_address << std::endl;
        std::cout << "DEFAULT I2C BUS SPEED: " << std::dec << m_bus_speed << " KHz" << std::endl;
        std::cout << "DEFAULT I2C INTERNAL BUS PULL-UP EN: " << m_internal_pull_up_en << std::endl;
    }

    // Constructor with Input Arguments
    i2c_slave(uint8_t slave_addr, uint32_t bus_speed, bool internal_pull_up_en){
        m_slave_address = slave_addr;               // Default Sensor Address
        m_bus_speed = bus_speed;                    //100 Khz - Standard Bus Speed
        m_internal_pull_up_en = internal_pull_up_en;     // Internal pull-up enable status
        std::cout << "[CONSTRUCTOR] I2C SENSOR INSTANCE INTIALIZED - WITH PARAMS: " << std::endl;
        std::cout << "I2C SENSOR ADDRESS: 0x" << std::hex << (uint)slave_addr << std::endl;
        std::cout << "I2C BUS SPEED: " << std::dec << bus_speed << " KHz" << std::endl;
        std::cout << "I2C INTERNAL BUS PULL-UP EN: " << internal_pull_up_en << std::endl;
    }

    // Default Destructor
    ~i2c_slave(){
        std::cout << "[DESTRUCTOR] I2C SENSOR INSTANCE RESOURCES RELEASED" << std::endl;
    }

    // APIs
    void i2c_read(){
        std::cout << "I2C SENSOR READ SUCCESS" << std::endl;
    }
    void i2c_write(){
        std::cout << "I2C SENSOR WRITE SUCCESS" << std::endl;
    }
};

// Inheritance Example below
class sensor_ina260 : public i2c_slave {
private:
    uint16_t m_ina260_calibration_data;
public:
    float voltage, current, power;
    // Default Constructor
    sensor_ina260(){
        std::cout << "[CONSTRUCTOR] INA260 SENSOR INITALIZED" << std::endl;
        m_ina260_calibration_data = 0x0000;
        voltage = 0.00f;
        current = 0.00f;
        power = 0.00f;
    }

    // Default Destructor
    ~sensor_ina260(){
        std::cout << "[DESTRUCTOR] INA260 SENSOR RESOURCES RELEASED" << std::endl;
    }

    void log_sensor_info(){
        std::cout << "INA260 SENSOR - DESIGNED BY TI - USED FOR PWR SENSING APPLICATIONS" << std::endl;
    }

    void fetch_calibration(){
        i2c_read();
        m_ina260_calibration_data = 0x2890;
        std::cout << "INA260 SENSOR CALIBRATION VALUE: 0x2890" << std::endl;
    }
};

int main(){
    // i2c_slave in219;
    i2c_slave ina219(0x44, 400000, true);

    sensor_ina260 charger_power_sensor;
    charger_power_sensor.fetch_calibration();

    std::cout << "SIZE OF I2C_SLAVE CLASS: " << sizeof(i2c_slave) << std::endl;
    std::cout << "SIZE OF SENSOR_INA260 CLASS: " << sizeof(sensor_ina260) << std::endl;
    
    std::cin.get();
}
```

**Note:** Upto this point classes, constructor, destructor, and basics of inheritance are learnt, so there might be somethings missing like virtual stuff, will focus on it when that topic is in-progress and wil lmodify the above code at that point.


## Virtual Functions in C++
- Virtual functions allow us to override methods in sub-classes.
- Virtual functions increase dynamic dispatch which compilers typically implement by our VTable.
- VTable is basically a table that contains the mapping of all the virtual functions aside our base class, so that we can actually map them to the correct overwritten function at runtime.
- Discussed about the `virtual` and `override` keywords in C++.
- Discussed the cost of using virtual functions:
    - The additional memory required in order for us to store the VTable so that we can dispatch to correct function that includes a member pointer that points to the base class.
    - Every time we call a virtual function, we have to go throught the VTable to determine which function to actually map to which is an additional performance penalty.

### Personal Notes
To understand why `virtual` is necessary, you first need to see how the C++ Compiler behaves when it gets confused by pointers.
1. The Problem: Static Dispatch (No Virtual)
    1. Imagine you are writing a hardware driver, and you have a base `Peripheral` class and a `SPI` Class that inherits from it.
        ```cpp
        #include <iostream>

        class Peripheral {
        public:
            void transmit() {
                std::cout << "Sending generic peripheral data..." << std::endl;
            }
        };

        class SPI : public Peripheral {
        public:
            void transmit() {
                std::cout << "Sending fast SPI data!" << std::endl;
            }
        };
        ```
    2. The Scenario: You create an `SPI` object, but you store its address inside a `Peripheral*` pointer. (this is incredibly common in firmware when you wanr an array of generic pointers pointing to different specific sensors).
        ```cpp
        int main(){
            SPI mySpibus;

            // Create a base pointer pointing to the derived object
            Peripheral* devPtr = &mySpibus;

            // What happens here?
            devPtr->transmit();
            return 0;
        }
        ```
    3. The Output: "Sending generic peripheral data"
    4. Why did it fail? because of static dispatch, the compiler looked at `Peripheral* devPtr` and saw the type was `Peripheral*` and hardcoded a direct assembly jump to `Peripheral::transmit()`.
    5. It completely ignored the fact that the actual object sitting in memory was an `SPI` object.
2. The Solution: Dynamic Dispatch (Using `virtual`)
    1. To fix this, we add the `virtual` keyword to the base class, and the `override` keyword to the derived class.
        ```cpp
        #include <iostream>

        class Peripheral {
        public:
            // Virtual Tells the compiler: "Check the VTable at runtime!"
            virtual void transmit() {
                std::cout << "Sending generic peripheral data..." << std::endl;
            }
        };

        class SPI : public Peripheral {
        public:
            // override tells the compiler: "Make sure i am actually overriding a virtual base function"
            void transmit() override {
                std::cout << "Sending fast SPI data!" << std::endl;
            }
        };
        ```
    2. Now if we run the same `main()` function, The Output: "Sending fast SPI Data!"
    3. Why did it work? the compiler saw `virtual`, so it did not hardcode a jump, instead it injected assembly instruction to say "When we hit this line, look at the objecy `devPtr` is pointing to, find its hidden `_vptr`, follow it to the `SPI` VTable, and run whatever fucntion address is sitting there".
3. The True Cost (Memory & Speed) - As a system engineer, you need to know exactly  what you are paying for this flexibility.
    1. The Memory Penalty - When you add a virtual function, the compiler silently changes your memory layout:
        1. The VTable (ROM/FLASH): The compiler creates an array of function pointers for the `SPI` class. this exists exactly once per class type. (Cost: ~4-8 bytes per virtual function per class)
        2. The vptr (RAM): The Compiler injects a hidden pointer (`_vptr`) into the actual `SPI` object instance.
            1. If `SPI` had an `int baud_rate` (4 bytes), `sizeof(SPI)` without virtual functions would be 4 bytes.
            2. With virtual functions on a 64-bit system, `sizeof(SPI)` becomes 12 bytes (4 Bytes for the int + 8 Bytes for the hidden `_vptr`)

    2. The Speed Penalty
        1. To Call `devPtr->transmit()`, the CPU must perform extra work:
            1. Fetch the Object's `_vptr` from RAM.
            2. Dereference the _vptr` to find the VTable in FLASH.
            3. Read the function address from the VTable.
            4. jump to that function address.
        2. This indirect jumping can cause Pipeline Stalls or Cache Misses in your CPU.

## Interfaces in C++
- Discussed about a type of virtual function called a "pure virtual function".
- A pure virtual function allows us to define a function in a base class that does not have an implementation, and then force subclasses to actually implement that function.
- Discusses on why programmers create base class that consists of only unimplemented methods, and the forc a subclass to actually implement them, and this is often referred to as an interface.
- An Interface is a class that only consists of unimplemented methods and acting as a template of sorts.
- An Interface class doesn't actually contain method implementations, therefore not possible to instantiate this type of class.
- Discussed the C++ Interface Hierarchy & Concrete Sub-Classes
    - An Interface (a class containing at least one pure virtual function) cannot be instantiated. Any sub-class inheriting from it must implement those pure virtual functions to become a concrete class (a class you can actually create objects from).
    - However, once a sub-class fully implements those pure virtual functions, the requirement is satisfied for the entire branch of that family tree. Any further sub-classes down the chain inherit that implementation and are instantly concrete—they do not have to re-implement the interface functions unless they want to override them.

### Personal Notes
1. In C++, an Interface isn't a special keyword.
2. An Interface is simply a standard C++ `class` where at least one virtual function is set to `=0`. We cann this an Abstract Class.
3. The Syntax & The Hardware Contract
    1. When you append `=0` to a virtual function, you are creating a "Pure virtual Interface".
        ```Cpp
        // The 'I' prefix is a naming convection meaning "Interface"
        class ICommBus {
        public:
            // pure Virtual Function: "I will not write this code. You must."
            virtual void transmit(uint8_t data) = 0;

            // Always include a virtual destructor in an Interface!
            virtual ~ICommBus() = default;
        };
        ```
    2. The VTable Reality of `=0`: why `=0`? >> It is actually a literal instruction to the compiler.
    3. It tells the compiler: "Put a `NULL` pointer in the VTable for this function".
    4. Becasue the VTable contains a `NULL` pointer, if you tried to create an `ICommBus` object and call `transmit()`, the CPU would jumpt to `NULL` and the system would instanty crash (Segmentation Fault).
    5. This is why the compiler strictly forbids you from instantiating an Interface.
4. Enforcing the Contract (The Concrete Class)
    1. When a subclass inherits from an interface, it signs a contract. If it does not provide an implementation for every single pure virtual function, the compiler treats the subclass as an Interface too, and refuses to let you instantiate it.
    2. Scenario A: The Broken Contract
        ```cpp
        class I2CBus : public ICommBus {
        public:
            void init() {
                // Did some setup, but forgot to implement transmit()!
            }
        };

        // COMPILER ERROR! I2CBus is still an abstract class because transmit() is missing.
        I2CBus myI2C; 
        ```
        1. E.g. Firmware benefit: this is a massive safety net. if a junior engineer tries to write a new SPI driver but forgets to implement the core `transmit` function, this code simply won't compile.
    3. Scenario B: the Fullfilled Contract (Concrete Class)
        ```cpp
        class I2CBus : public ICommBus {
        public:
            // The contract is fulfilled! The compiler is happy.
            void transmit(uint8_t data) override {
                // Code to write data to the STM32 I2C Data Register...
            }
        };

        // SUCCESS! You can now create objects of this type.
        I2CBus myI2C; 
        ```
    4. The Family Tree (Concrete Propagation)
        1. Once a sub class fully implements those pure virtual functions, the requirement is satisfied for the entire branch of that family tree.
        2. For E.g. you want to make a specialized version of your i2c bus that automatically retries if the transmission fails.
        ```cpp
        // Family Tree Example: FastI2CBus inherits from I2CBus (which already fullfilled the ICommBus contract)
        class FastI2CBus : public I2CBus {
        public:
            void setFastMode() {
                std::cout << "I2C BUS FAST MODE SET" << std::endl;
            }

            // Note that we did not implement transmit() here
        };

        int main(){
            std::cout << "==== TOPIC: INTERFACES IN C++ ====" << std::endl;

            FastI2CBus myFastBus; //Instantiate - SUCCESS

            myFastBus.setFastMode();
            // The above instance automatically uses the transmit() implementation which it inherited from the parent I2CBus Cass
            myFastBus.transmit(0xff);

            std::cout << "==================================" << std::endl;
            std::cin.get();
        }
        ```
        3. Because `I2CBus` provided the memory address for `transmit()` to the VTable, `FastI2CBus` inherits a fully populated VTable (no `NULL` POINTERS).
        4. It is instantly a Concrete Class without you having to write any extra boilerplate code.
        5. If `FastI2CBus` wanted to change how transmitting works, it could imply `override` the function again, replacing the Parent's VTable entry with its own.


## Visibility in C++
- Discussed visibility in Classes.
- Visibility concept implementation in classes does not have any effect of performace of speed in runtime.
- Visibility in C++ has 3 basic modifiers i.e. `public`, `protected`, `private`.
- Discussed about `friend` keyword.
- When using `public` keyword in a class, all attributes and methods under the keyword is accessable to outside code by calling the `instance.attribute` or `instance.method()`.
- When using the `private` keyword in a class all attributes and methods under the keyword is not accessable to outside code and can only be used by the class and the programmer might need to create public methods to do any operations on the private data attributes or methods. Note: even sub-classes cannot call these data attributes or methods directly.
- When using `protected` keyword in a class, all attributes and methods under the keyword can be accessed by the class and all sub-classes, but no access to outside code to those protected data attributes and methods.
- Discussed few example use-cases/application of visibility modifiers in C++

---

## Separate Compilation in C++ (Reference Textbook Ch3:3.2)
- C++ supports a notion of separate compilation where user code sees only declarations of the types and functions used.
- The definitions of those types and functions are in separate source files and compiled separately.
- This can be used to organize a program into a set of semi-independent code fragments, such separation can be used to minimize compilation times and to strictly enforce separation of logically distinct part of a program, thus minimizing the chance of errors.
- A Library is often a collection of separately compiled code fragments (e.g. functions).
- E.g. Implementation of a Header file and Source file for Vector class code design
    - Programmers will implement a header file `vecotr.h`
    ```cpp
    // vector.h
    class Vector{
    private:
        double* elem;   // elem points to an array of sz double
        int sz;
    public:
        Vector(int s);
        double& operator[](int i);
        int size();
    };
    ```
    - Programmers will then implement a `vector.cpp` file that will house the implementation of the header file declarations
    ```cpp
    // Vector.cpp
    #include "vector.hpp"

    Vector::Vector(int s) : elem{new double[s]}, sz{s} {}

    double& Vector::operator[](int i) {
        return elem[i];
    }

    int Vector::size() {
        return sz;
    }
    ```
    - Note: Compared to C, C++ in the above implementation si doing three massive things i.e. Scope binding, Pre-body Initialization, and Operator Overloading.
        - The Scope Resolution Operator (`::`): In C++, `::` tells the compiler: "I am defining a function, but it strictly belongs to the `Vector` class blueprint defined in the header". It implicity passes the pointer to the object behind the scenes, so you dont have to pass it as an argument, like its done in C i.e. `void Vector_Init(Vector_t* v, int s)`.
        - The Member Initializer List (`:`): In C++, there is a strict difference between Initialization (giving memory a value the moment it is created) and Assignment (overwriting existing memory with a new value).
            - The Code after the colon `:` runs before the code inside the `{}`.
            - It initalizes `elem` and `sz` directly into memory.
            - If you did it inside the `{}` (like its done in C), C++ would first creare the variables with garbage values, and then you would overwrite them.
            - The Initalizer List skips the garbage step, saving CPU cycles. it is standard practice in modern C++.
        - The Dynamic Memory (`new` vs `malloc`): C++ introduces `new` which calculates the size for you based on the type, handles the type-casting automatically, and if you were creating an array of objects rather than standard `double`, it would automatically call the constructors for every single object in the array.
        - The Operator Overloading & references: `double& Vector::operator[](int i)` - this is pure C++, in C, you cannot change what mathematical symbols or brackets do. To get an element, you'd have to write a function like `double get_item(Vector* v, int i)`, but by naming the function `operator[]`, C++ allows you to use your custom class like a native array, for e.g
        ```cpp
        Vector myVec(10);
        myVec[3] = 42.5; // this secretly calls your operator[] function!
        ```
        - Note: by returning a `&` Reference, you hand the exact memory address abck to the caller, allowing them to overwrite the value.
    - Programmers will include the header file `vector.h or .hpp` in main.cpp to access vector implementation

## Namespaces in C++
- C++ offers namespaces as a mechansim for expressing that some declarations belong together and that their names shouldn't clash with other names.
- In C, the concept of a "namespace" doesn't exist. All functions and global variables live in one giant, global pool.
- If you are writing firmware in C and you have a UART driver and an SPI Driver, you cannot name there initialization functions `init()`. The linker will throw a "Multiple Definition" error because it sees two `init` symbols in the global table. To fix this, you use prefixes: `UART_init()` and `SPI_init()`.
- In C++, Namespaces exist to solve exactly this problem. A namespace is simply a named box that you put your code inside to keep it isolated from the rest of the project.
- Note: The Scope Resolution Operator `::` is used with namespace
- How to define a namespace:
```cpp
#include <iostream>
#include <stdint.h>

namespace UART {
    int baud_rate = 115200;
    
    void init(){
        std::cout << "UART Peripheral Initialized" << std::endl;
    }

    void transmit(char data){
        std::cout << "Transmitting data via UART: " << data << std::endl;
    }
}

namespace SPI {
    int clock_speed = 400'000;
    
    void init(){
        std::cout << "SPI Peripheral Initialized" << std::endl;
    }

    void transmit(char& data){
        std::cout << "Transmitting data via SPI: 0x" << std::hex << (int)(unsigned char)data << std::endl;
    }
}


int main(){
    std::cout << "==== Topic: Namespaces in C++ ====" << std::endl;
    char spi_byte = 0xAA;
    UART::init();
    SPI::init();

    UART::transmit('A');
    SPI::transmit(spi_byte);

    std::cout << "==================================" << std::endl;
    std::cin.get();
}
```
- C++ offers the `using` keyword to remove UART::<> or SPI::<> while using the namespace box definitions.

## Error Handling in C++
- C++ introduces Exceptions with `try`, `catch`, and `throw` keywords to ensure errors cannot be ignored. If an error occurs and nobody handles it, the program stops immediately.
- In C++, when a function detects a problem that it cannot solve on its own, it throws an exception. It is essentially hitting the emergency stop button and saying, "I can't continue. Whoever called me needs to deal with this."

### exceptions
- The Core Syntax: `throw`, `try`, and `catch`:
    - Instead of returning a -1 status code, we use the `throw` keyword. To handle the thrown error safely, the caller wraps the dangerous code in a `try` block, and `catches` the error in a catch block.
    ```cpp
    #include <iostream>
    #include <stdexcept> // Standard C++ exception types

    // A mock function that might fail
    int readSensorData(int pin) {
        if (pin < 0 || pin > 15) {
            // We throw an exception object instead of returning -1
            throw std::invalid_argument("Hardware error: Invalid pin number!");
        }
        
        return 1024; // Simulated valid reading
    }

    int main() {
        try {
            // The compiler attempts to run this code
            std::cout << "Reading pin 5: " << readSensorData(5) << std::endl;
            
            // This will trigger the exception!
            std::cout << "Reading pin 99: " << readSensorData(99) << std::endl; 
            
            // This line will NEVER execute because the line above threw an exception.
            std::cout << "All sensors read successfully." << std::endl;
            
        } catch (const std::invalid_argument& e) {
            // If an invalid_argument exception is thrown anywhere in the 'try' block, 
            // the CPU jumps immediately here.
            std::cout << "CAUGHT EXCEPTION: " << e.what() << std::endl;
        }
        
        return 0;
    }
    ```
    ```text
    Output Log:
    Reading pin 5: 1024
    Reading pin 99: CAUGHT EXCEPTION: Hardware error: Invalid pin number!
    ```
### Invarients
- Why use Exceptions instead of Error Codes? (The Constructor Problem)
    - In Object-Oriented C++, Constructors do not have return types.
    - If you try to creare a `RingBuffer` object, but the system doesn't have enough RAM to allocate the array, how does the constructor tell `main()` that it failed? it can't return `false`.
    - The Creator of C++ call this establishing an **Invarient** which means that a rule that must be true for the object to exist.
    - If the Constructor cannot establish the invarient, it must throw an exception. this guarantees that if an object existsm it is completely valid and safe to use.
    - Example code:
        ```cpp
        class NetworkSocket {
        public:
            NetworkSocket(int port) {
                if (port < 1024) {
                    throw std::runtime_error("Cannot bind to privileged port!");
                }
                // Bind port...
            }
        };

        int main() {
            try {
                NetworkSocket mySock(80); // Fails to construct! Throws exception.
            } catch (const std::runtime_error& e) {
                std::cout << "Failed to start server: " << e.what() << '\n';
            }
        }
        ```
        ```text
        Output Log:
        Failed to start server: Caught Exception: Cannot bind to privileged port!
        ```
- Firmware Compatibiltiy check (No exception mode)
    - While A Tour of C++ teaches exceptions as the gold standard, firmware and game engine developers often ban them.
    - If you look at the compilation flags for many STM32, Arduino, or high-performance game engine projects, you will see this flag: `-fno-exceptions`.
    - Why do embedded engineers turn exceptions off?
        - Flash Memory Bloat (Stack Unwinding):
            - To make try/catch work, the compiler must inject massive "Exception Tables" into your binary.
            - When an exception is thrown, a hidden C++ runtime function must walk backward through the call stack (called "unwinding"), figuring out exactly which local objects need their destructors called before it finally lands in your catch block.
            - This metadata can bloat your `.bin` file by 10% to 30%, which is catastrophic on a microcontroller with only 64KB of total Flash memory.
    - Unpredictable Timing (Non-Deterministic execution):
        - In a real-time operating system (RTOS) or a high-speed hardware interrupt (like a 1kHz drone motor PID loop), you need absolute, deterministic execution times.
        - The process of searching up the call stack for a catch block takes an unpredictable amount of CPU cycles.
        - If a sensor error takes 5 microseconds to handle today, but 150 microseconds tomorrow because the stack was deeper, your drone's control loop will miss its deadline and crash.
- What do we use instead for firmware?
    - If you compile with -fno-exceptions, you go back to the C-style philosophy (returning errors), but you use modern C++ safety wrappers instead of raw -1 integers.
    - **Alternative 1:** Error Structs (The simple C++ Way)
        - Instead of passing pointers to get multiple values back like in C, we return a lightweight struct containing both the data and a strictly-typed error enum. 
        ```cpp
        enum class SensorStatus { OK, I2C_ERROR, TIMEOUT };

        struct SensorResult {
            float temperature;
            SensorStatus status;
        };

        SensorResult readTemp() {
            // Hardware read failed! Return 0.0 for data, and the specific error flag.
            return { 0.0f, SensorStatus::I2C_ERROR }; 
        }
        ```
    - **Alternative 2:** `std::optional` (C++17)
        - Perfect for when a function might just fail to find something.
        - It returns a wrapper that holds "either the value, or literally nothing."
        - It forces the caller to check if the data exists before using it.
        ```cpp
        #include <optional>

        std::optional<int> getBufferData() {
            if (buffer_empty) return std::nullopt; // Fails safely
            return 42; // Succeeds
        }

        // In main.cpp:
        auto data = getBufferData();
        if (data.has_value()) {
            std::cout << data.value();
        }
        ```
    - **Alternative 3:** `std::expected` (C++23)
        - The modern holy grail for firmware.
        - It holds either the expected return value or a specific error code (very similar to Rust's famous Result type).
        - It has zero memory overhead compared to exceptions.

- The notion of invariants is central to the design of classes, and preconditions serve a similar role in the design of functions.
- Invarients helps us to understand precisely what we want, and forces us to be specific; that gives us a better chance of getting our code correct(after debugging and testing).
- The notion of invarients underlies C++'s notions of resource management supported by contructors and destructors.

### Static Assertions - Compile-Time Error Handling (`static_assert`)
- Exceptions report errors found at run time. Everything discussed above handles Runtime Errors.
- C++ also provides a mechanism for Compile-Time Error handling using the `static_assert` keyword. this tells the compiler: "If this mathematically condition is false, fail the build immediately."
- Because `static_assert` is evaluated exclusively by the compiler, it generates zero assembly code and has absolutely no CPU or Memory cost.
- The Firmware Use-Case: Memory Mapping
    - In firmware, you often map C++ structs directly over hardware registers.
    - If a 32-bit register bank requires exactly 12 bytes of data, but someone accidentally adds an extra `uint32_t` to the struct, the struct becomes 16 bytes.
    - When you write this to the hardware, the system will crash or behave erratically.
    - You can use `static_assert` to make the compiler guard your hardware constraints:
    ```cpp
    #include <cstdint>

    // Simulating a memory-mapped hardware register configuration
    struct SpiRegisters {
        uint32_t control;
        uint32_t status;
        uint32_t data;
    };

    // The compiler checks this before building the binary!
    // If someone adds a variable to SpiRegisters, the build will instantly fail.
    static_assert(sizeof(SpiRegisters) == 12, "FATAL: SpiRegisters struct must be exactly 12 bytes to match hardware!");

    int main() {
        // Normal code here...
        return 0;
    }
    ```
    - By putting `static_assert` checks throughout your firmware, you catch configuration errors, architecture mismatches, and alignment issues before you ever flash the board.

- Note: Chapter 3 is completed wrt topic coverage.
---

## Member Initializer Lists in C++
- Constructor member intialization lists is a way for us to initialize our class member functions in the constructor.
- Discussed Member Initializer lists and hands on implementation of constructor member intializer list, and emphasized on order in the member initializer list.
- Discussed what happens when Member Initializer are not used vs when they are used.

### Personal Notes
A Member Initializer List is a syntax used in constructor definitions to initialize class member variables before the constructor body executes. It is placed after the constructor parameter list, starting with `:`.

**Syntax & The "Two-Step" Trap (without vs with)**

To Understand why this feature exists, lets look at what compiler does behind the scene when you initialize a custom object (like a hardware peripheral driver) inside another class.

Scenario A: Wthout Initializer List (The C Style habit)

```cpp

```