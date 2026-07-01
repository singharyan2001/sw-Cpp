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

Below is an example code written, and we will be toggling the `MODERN_PATTERN` enable/disable to see how it affects initialization.
So by Disabling the `MODERN_PATTERN` by setting it to 0, we enable the Anti-Pattern to see what happens because of this in the output log.
```cpp
// Member Initializer List in C++
#include <iostream>

#define MODERN_PATTERN_CPP  0

class UartDriver {
public:
    UartDriver(){
        /*Default constructor: Boots in low-power mode*/
        std::cout << "DEFAULT CONSTRUCTOR: BOOTINT IN LOW POWER MODE" << std::endl;
    }
    UartDriver(int baud){
        /* Configures high-speed baud rate */
        std::cout << "DEFAULT CONSTRUCTOR: BOOTINT IN NORMAL MODE AT BAUD RATE: " << baud << std::endl;
    }

    void setBaud(int b){
        /* Overwrites baud rate */
        std::cout << "SETTING BAUD RATE TO: " << b << std::endl;
    }
};

class FlightController {
private:
    UartDriver gps_uart; // A Custom class member
    int error_count;
public:
#if MODERN_PATTERN_CPP
    // Modern-Pattern: Direct Memory Intialization
    FlightController(int baud_rate) : gps_uart(baud_rate), error_count(0) {
        // Constructor Body is now empty - and creation & Initialization is done only once now.
    }
#else
    // Anti-pattern Assigning inside the constructor body
    FlightController(int baud_rate){
        gps_uart = UartDriver(baud_rate);
        error_count = 0;
    }
#endif
};

int main(){
    std::cout << "========== TOPIC: Member Intializer List in C++ ==========" << std::endl;
    FlightController fc(9600);

    std::cout << "==========================================================" << std::endl;
    std::cin.get();
}
```
```text
Output Log:
========== TOPIC: Member Intializer List in C++ ==========
DEFAULT CONSTRUCTOR: BOOTINT IN LOW POWER MODE
DEFAULT CONSTRUCTOR: BOOTINT IN NORMAL MODE AT BAUD RATE: 9600
==========================================================
```

What actually happened in CPU Cycles? Because we didn't use an intializer list, C++ executes a Hidden Step 0 before constructor `{}` block runs.
1. Step 0 (hidden): The Compiler allocates `gps_uart` ad calls its default constructor `UartDriver()`.
2. Step 1 (Inside body): It Created a Temporary `UartDriver(baud_rate)` object.
3. Step 2: The Compiler uses the Assignment Operator `=` to copy the temporary object into `gps_uart`.
4. Step 3: The Temporary object is destroyed.
5. ResultL YOu booted the peripheral twuce and wasted memory allocations.

Now by enabling the `MODERN PATTERN` by setting 0 -> 1, we can totally see a different result:
```text
Output Log:
========== TOPIC: Member Intializer List in C++ ==========
DEFAULT CONSTRUCTOR: BOOTINT IN NORMAL MODE AT BAUD RATE: 9600
==========================================================
```

What happened here? the compiler directly constructs `gps_uart` in its final memory address using `UartDriber(baud_rate)`, it skips step 0 and therefore the results obtained are Zero wasted cycles of the CPU and the Default Constructor is completely bypassed.

**When Initializer Lists are Mandatory**

In Modern C++, there are three scenarios where the compiler will flat-out refuse to build the project if you dont use an Initializer Lists:
1. `const` Member Variables: Once memory is marked `const`, it cannot be assigned to via `=`. It must receive its value at the exact microsecond of its birth.
```cpp
// Const example for member intializer list
class Sensor {
private:
    const int i2c_address;
public:
    // ERROR: Cannot assign to variable 'i2c_address' with const-qualified type.
    // Sensor(int addr) { i2c_address = addr; }
    
    // SUCCESS: Initialized at birth
    Sensor(int addr) : i2c_address(addr) {
        // Empty constructor
    }
};
```
2. `reference` Member Variables: just like `const`, C++ references cannot be "empty" or re-seated. They must be intialized immediately.
```cpp
// reference example for member intializer list
class DataLogger {
private:
    int& rx_buffer; // Reference to an external buffer
public:
    DataLogger(int& buf) : rx_buffer(buf) {
        // Empty Constructor
    }
};
```
3. Objects without a default constructor: If you use a third party object that only has a constructor requiring arguments (no `Object()`), you cannot construct it inside the `{}` body.

**The Declaration Order Trap**

Rule: Class Members are always initialized in the exact order they are declared in the class definition (the `.h` file), completely ignoring the order you type them in the initalizer List.

Note: If the rule is not followed then there could be unefined behaviour of the object, for e.g.
```cpp
class BadActuator {
private:
    int raw_scaling_factor;
    int calibrated_speed;
public:
    // DANGER! Looks innocent, but causes undefined behavior.
    BadActuator(int input) : calibrated_speed(raw_scaling_factor * 2), raw_scaling_factor(input) {}
};
```

Why does this crash? when we look at the `private` section, the `raw_scaling_factor` is declared first. Therefore, the compiler intializes it first.
1. The Compiler intializes `raw_scaling_factor`. But in the constructor member list, the value is not given yet, so it gets unintialized garbage RAM data.
2. The Compiler then initializes `calibrated_speed` using `raw_scaling_factor * 2`
3. The Compiler finally sets `raw_scaling_factor = input`
4. Now the `calibrated_speed` is corrupted.

What can be done to avoid this? the fix could be enabling the `-Wreorder` flag in the GCC/CMake Compiler, since modern compilers will warn you if your intializer list doesn't match your header declaration order.

## const in C++
- Discussed const keyword
- Discussed using const with pointers
- Discussed using const with classes.
- Discussed const pointer return with pointer contents const as well and the function is also const, therefore no modifications in the function - `const int* const GetX() const {}`.
- Discussed using const with reference and passing arguments as const. (e.g. `void printEntity)const Entity& e){}` )
- Discussed calling const functions from classes and having identical function with const and no const function.
- Discussed mutable keyword and use case inside a const function.
- The Cherno Explains how the const keyword functions as a promise in C++ to prevent accidental modifications to variables, pointers, and class members. Learn to improve code safety by applying const to methods and understanding the difference between constant pointers and pointers to constants.

### Personal Notes
In C++, `const` is a mechanism to enforce read-only safety. Once you label something `const`, the compiler will aggressively throw errors if you or anyone else attempts to modify it.

**Pass by const reference**

This is the most common use of `const`, when passing a large struct (like a 100-byte `SensorPayload`) to a function, passing by value creates a slow, 100 byte memory copy.
Passing by reference(`&`) is fast (it just passes the 8-byte memory address), but it gives the function the power to accidently overwrite data.

The solution is to combine both `reference` and `const`.
```cpp
// const in C++
#include <iostream>

struct SensorPayload {
    float temp[20];
    int timestamp;
};

// FAST (no copy) and SAFE (read-only)
void printPayload(const SensorPayload& payload){
    std::cout << payload.timestamp << std::endl;
    
    // COMPILER ERROR: Cannot assign to variable 'payload' with const-qualifed type
    // payload.timestamp = 0;
}

int main(){
    std::cout << "========== TOPIC: const in C++ ==========" << std::endl;
    const SensorPayload data = {
        {24.6, 27.3},
        123443
    };
    printPayload(data);

    std::cout << "=========================================" << std::endl;
}
```

**The Pointer Puzzle (Read Right-to-Left)**

When combining `const` with pointers(`*`), the golden rule of C++ is to read the declaration beackward (right to left).

1. A Pointer to a Constant (`const int* ptr`)
    1. Read Right-to-Left: `ptr` is a pointer(`*`) to an integer(`int`) that is constant(`const`)
    ```cpp
    // EXAMPLE: A Pointer to a Constant (data)
    int a = 5;
    int b = 10;
    const int* ptr = &a;    // ptr is a pointer to an integer that is constant.
    ptr = &b;               // OK: You can change WHERE the pointer points.
    //*ptr = 7;             // ERROR: You cannot change the VALUE it points to.
    ```
2. Constant Pointer (`int* const ptr`)
    1. Read Right-to-Left: `ptr` is a pointer that is constant(`const`) to an integer(`int`)
    ```cpp
    // EXAMPLE: Constant Pointer
    int* const ptr2 = &a;   // ptr2 is a pointer that is constant to an integer.
    *ptr2 = 7;              // OK: You can change the data WHERE the pointer is pointing to.
    //ptr2 = &b;              //ERROR: You cannot change WHERE the pointer is pointing to.
    ```
3. Constant Pointer to a Constant (`const int* const ptr`)
    1. Read Right-to-Left: `ptr` is a pointer that is constant(`const`) to an integer(`int`) that is constant(`const`).
    ```cpp
    // EXAMPLE: Constant Pointer to a Constant (data)
    const int* const ptr3 = &b; // ptr3 is a pointer that is constant to an integer that is constant.
    //*ptr3 = 20;           // ERROR: You cannot change the VALUE the pointer points to.
    //ptr3 = &a;            // ERROR: You cannot change WHERE the pointer points to.
    // // Completely locked down. You can only read it.
    ```

**Const Methods in Classes**

1. When you put `const` at the end of a class method, you are making a promise: "This method will not modify any member variables of this class."
    ```cpp
    // Class Methods in Classes
    class UartDriver {
    private:
        int baud_rate = 115200;
    public:
        // A normal Method (can modify state)
        void setBaud(int new_baud){
            baud_rate = new_baud;
        }

        // A Const Method (Read-Only)
        int getBaud() const {
            // baud_rate = 9600; // COMPILER ERROR: Method is const!
            return baud_rate;
        }
    };
    ```
2. Why does this matter? if you pass a `UartDriver` to a function as a `const UartDriver&`, the compiler will only let you call methods that have `const` at the end! It protects the Object.
3. Example: `const int* const GetX() const {}`:
    1. Function Syntax: `<return-type>``function-name``Qualifier`{}
    2. `const int* const` (The Return Type): It returns a pointer, and the pointer itself cannot be redirected to a new address, and the integer it points to cannot be modified.
    3. `GetX()` (The Name): The Function Name.
    4. `const` (The Method Qualifier): Calling this function will not modify any variables inside the class it belongs to.

**Const Overloading** 
1. You can actually have two functions with the exact same name and arguments, where the only difference is the const keyword at the end.
2. The compiler is smart enough to pick the correct one based on whether the object is const or not.
3. E.g.
    ```cpp
    class Buffer {
    private:
        int data[10];
    public:
        // Version 1: For normal objects (Returns writable reference)
        int& get(int index) { return data[index]; }
        
        // Version 2: For const objects (Returns read-only reference)
        const int& get(int index) const { return data[index]; }
    };
    ```

## The Mutable keyword
- Discussed on mutable keyword.
- Discussed mutable keyword's 2 main use cases i.e. `const` and `lambda` in C++.

### Personal Notes
1. Sometimes, you have a `const` method (like reading a sensor), but you need to modify a hidden "background" variable, like a debug counter or a hardware cache.
2. If a method is `const`, it can't modify anything. To bypass this for a specific variable, you mark that variable as a `mutable`.
    ```cpp
    #include <iostream>

    // EXAMPLE: Mutable Keyword in a const method in a class
    class TemperatureSensor {
    private:
        int i2c_address;
        mutable int read_count_since_boot = 0;  // Allowed to change even in const methods!
    public:
        TemperatureSensor(int addr) : i2c_address(addr) {
            std::cout << "Temperature Sensor Object Instantiated Successfully" << std::endl;
        }

        // A Const Method - to promise that no changes will be done in the function/method
        float getTemp() const {
            // Read Sensor
            std::cout << "Temperature Read Success!" << std::endl;
            // update Read Count since boot variable
            read_count_since_boot++;
            // i2c_address = 0x44; // ERROR: Cannot be modifed inside a const method.
            return 25.3f;
        }

        int getCount() const{
            std::cout << "Read Count Since Boot: " << read_count_since_boot << std::endl;
            return read_count_since_boot;
        }
    };

    int main(){
        std::cout << "========== TOPIC: mutable keyword in C++ ==========" << std::endl;

        // EXAMPLE: Accessing Constant methods & use case of a mutable keyword.
        TemperatureSensor sensor(0x40);
        sensor.getCount();
        for(int i=0; i < 5; i++){
            sensor.getTemp();
            sensor.getCount();
        }

        std::cout << "=========================================" << std::endl;
        std::cin.get();
    }
    ```
    ```text
    Output Log:
    ========== TOPIC: const in C++ ==========
    1782835800
    Temperature Sensor Object Instantiated Successfully
    Read Count Since Boot: 0
    Temperature Read Success!
    Read Count Since Boot: 1
    Temperature Read Success!
    Read Count Since Boot: 2
    Temperature Read Success!
    Read Count Since Boot: 3
    Temperature Read Success!
    Read Count Since Boot: 4
    Temperature Read Success!
    Read Count Since Boot: 5
    =========================================
    ```

## Ternary Operators in C++ (Conditional Assignment)
- Discussed on Ternary Operators
- Syntax: `result = condition ? value if true : value if false`

## How to Create/Instantiate Objects in C++
- Discussed about instantiating objects and how memory plays a role in this via Stack and Heap memory.
- Discussed about object instance, stack memory and automatic lifetime.
- Discussed about object instance, heap memory and dynamic object lifetime.
- Discussed about the `new` and `delete` keyword.
- Discussed performance impact, memory consumption, managing object lifetime for stack and heap based memory usage.
- Sumamry: The Cherno breaks down the differences between stack and heap memory allocation for object creation in C++. Learn when to choose automatic lifespan on the stack versus manual management on the heap, covering performance implications and syntax.

### Personal Notes

In C++, a `class` is just a blueprint. Instantiation is the act of taking that blueprint, asking the OS for memory, and running the constructor to breathe life into the object.
There are two completely different places in memory you can put an object: the Stack and The Heap.

**The Stack Memory (Automatic Lifetime)**
1. The Stack is the default, preferred, and fastest way to create an object in C++.
2. The Syntax: Unlicke Java or C#, you do not use the `new` keyword to create standard objects in C++. you simply declare them like standard variables.
3. E.g.
```cpp
class UartDriver {
public:
    UartDriver() { std::cout << "UART Booting...\n"; }
    ~UartDriver() { std::cout << "UART Shutting down...\n"; }
    void send() { /* ... */ }
};

void transmitData() {
    // 1. Instantiation: Object created on the Stack. Constructor runs IMMEDIATELY.
    UartDriver myUart; 
    
    // 2. Use the object (using the standard dot '.' operator)
    myUart.send();
    
    // 3. The function ends. 
    // The CPU automatically pops 'myUart' off the stack.
    // The Destructor (~UartDriver) runs AUTOMATICALLY right here!
}

int main(){
    std::cout << "========== Topic: Object Instantiation in C++ ==========" << std::endl;

    // EXAMPLE: Stack Memory based Object Instantiation
    transmitData();
    
    std::cout << "========================================================" << std::endl;
    std::cin.get();
}
```
```text
Output Log:
========== Topic: Object Instantiation in C++ ==========
UART Booting...
Data Sent via UART
UART Shutting down...
========================================================
```
4. The Firmware Reality of the Stack
    1. Performace: Blazing fast, Allocating an object on the stack requres a single CPU assembly instruction (moving the stack pointer)
    2. Memory Management: 100% Automatic, you cannot have a memory leak with a stack object.
    3. The Danger(Stack Overflow): The Stack is tiny. On a RPI running linux its usually 8MB, on stm32 mcu it might only be 2KB, so if you create a massive object on the stack (e.g. `class ImageBuffer {uint8_t pixels[1000000]; };`), your system will crash.

**The Heap memory (Dynamic Lifetime)**
1. The Heap is a massive pool of available RAM. You use the Heap when your object is too large to fit on the stack, or when you need the object to survive long after the function that created it has finished.
2. The Syntax: to put an object on the Heap, you use the `new` keyword. this acts exactly like C's `malloc()`, but automatically calculates the size and forces the constructor to run. Because it's on the Heap, `new` returns a Pointer to the memory address.
```cpp
// Object Instantiation in C++
#include <iostream>

class UartDriver {
public:
    UartDriver(){
        std::cout << "UART Booting..." << std::endl;
    }

    ~UartDriver(){
        std::cout << "UART Shutting down..." << std::endl;
    }

    void send(){
        std::cout << "Data Sent via UART" << std::endl;
    }
};

void transmitData(){
    // 1. Instantiation: Object created on the Stack. Constructor runs IMMEDIATELY.
    UartDriver myUart;

    // 2. Use the Object (using the standard dot '.' operator)
    myUart.send();

    // 3. The function ends.
    // The CPU automatically pops myUart off the stack.
    // The Destructor (~UartDriver) runs AUTOMATICALLY right here!
}

UartDriver* createPersistentDriver(){
    // 1. Instantiation: Ask for heap memory and run the Constructor
    UartDriver* heapUart = new UartDriver();

    // 2. Use the object (using the arrow '->' operator because it's a pointer)
    heapUart->send();

    // 3. the function ends, BUT the object survives!
    return heapUart;
}

int main(){
    std::cout << "========== Topic: Object Instantiation in C++ ==========" << std::endl;

    // EXAMPLE: Stack Memory based Object Instantiation
    transmitData();

    // EXAMPLE: Heap Memory based Object Instantiation
    UartDriver* myGlobalDriver = createPersistentDriver();
    
    // Use the driver for the entire lifetime of the program
    for(int i=0; i<10; i++) {
        myGlobalDriver->send();
    }

    // Manual Destruction is REQUIRED
    // if you forget this line, you have a Memory leak.
    // This frees the memory and runs the Destructor.
    delete myGlobalDriver;
    
    std::cout << "========================================================" << std::endl;
    std::cin.get();
}
```
```text
Output Log:
========== Topic: Object Instantiation in C++ ==========
UART Booting...
Data Sent via UART
UART Shutting down...
UART Booting...
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
Data Sent via UART
UART Shutting down...
========================================================
```
3. the Firmware Reality of the Heap
    1. Performance: Slow, Calling `new` forces the CPU to search through the Heap looking for a contiguous block of free memory. This takes hundreds of clock cycles.
    2. The Danger (Fragmentation): In a long-running embedded system (like a drone flying for 40 minutes), repeadly calling `new` and `delete` causes Heap Fragmentation. The Memory gets chopped into tiny, unusable blocks until `new` suddenly fails and the system crashes out of the sky.

**The Golden Rules of Instantiation**

A C++ systems engineer, memorizes these decision tree:
1. Default to the StackL if you can create the object normally, do it on stack since it is faster, safer, and cleans itself up.
2. Use the Heap (`new`) ONLY if:
    1. The Object is incredibly large (e.g. a 10MB network buffer).
    2. You explicitly need the object's lifetime to outlive the scope `{ }` in which it was created.

## The `new` Keyword in C++
- Discussed that the new keyword's main purpose is to allocate memory on the Heap Memory.
- Syntax: `new <datatype>`
- Discussed how the `new` keyword works in the background in C++.
- Discussed the primary takeaway of the new keyword i.e. it takes time (speed -> slow)
- Discussed how `new` keyword works with normal data types and classes.
- Discussed how `new` keyword and `malloc()` based implementation looks like and how the constructor needs to be manually called if `malloc()` is used (generally used in C wrt `malloc()`) if used in C++.
- Discussed that in C++, creating an object requires two distinct steps: allocating raw heap memory and initializing the object (running its constructor). The `new` keyword automates both steps, whereas the C-style `malloc()` function only handles the raw memory layout.
- Summary: The Cherno explores the fundamentals of dynamic memory allocation using the `new` keyword to place data on the heap. Learn how to allocate primitive types, arrays, and objects, while understanding the essential relationship between allocating memory and calling constructors in C++.

### Personal Notes
In C++, the `new` keyword is the primary operator used to allocate memory on the Heap (dynamic memory) at runtime. Unlike the Stack, where the compiler knows exactly how much memory is needed at compile-time, the Heap is used when you don't know how much memory you need until the program is actually running, or when you need an object to survive beyond the function that created it.

**Syntax and Basic Usage**

You can use `new` to allocate primitive types, arrays, and complex objects. It always returns a Pointer to the newly allocated memory address.
```cpp
// New and delete Keywords in C++
#include <iostream>

class UartDriver {
public:
    UartDriver(int baud) {
        std::cout << "UART Driver Baud rate SET: " << baud << std::endl;
    }
    
    ~UartDriver() {
        std::cout << "UART Driver Instance will now be destroyed!" << std::endl;
    }
};

int main(){
    std::cout << "========== Topic: NEW and DELETE Keyword in C++ ==========" << std::endl;

    // EXAMPLE: Primitive Types
    // Allocate exactly 4 bytes on the Heap (for a 32-bit int)
    int* myInt = new int;

    // Allocates 4 Bytes AND Initializes the value to 42.
    int* myInitializedInt = new int(42);

    std::cout << "myInt pointer pointing to data: " << *myInt << std::endl;
    std::cout << "myInitializedInt pointer pointing to data: " << *myInitializedInt << std::endl;

    //Critical Operation: Freeing allocated memory vua delete keyword
    delete myInt;
    delete myInitializedInt;

    // EXAMPLE: Arrays (Dynamic Buffers)
    // Allocates a contiguous block of 400 bytes (100 ints * 4 bytes)
    int* myBuffer = new int[100];
    std::cout << "Size of myBuffer Pointer: " << sizeof(myBuffer) << std::endl;
    int cal_size = 0;
    for(int i=0; i<100; i++){
        cal_size += sizeof(*myBuffer);
    }
    std::cout << "Size of myBuffer in bytes: " << cal_size << std::endl;
    //Critical Operation: Freeing allocated memory via delete keyword
    delete[] myBuffer;

    // EXAMPLE: Custom Objects (Classes)
    // Allocates memorty AND calls the constructor!
    UartDriver* myDriver = new UartDriver(115200);
    delete myDriver;    // Calls the Destructor, then frees memory

    std::cout << "==========================================================" << std::endl;
    std::cin.get();
}
```

**Under the Hood: `new` vs `malloc()`**

Creating an object requires two distinct steps (allocating raw memory + running the constructor). If you try to use C's `malloc()` to create a C++ object, you will get the memory, but the cosntructor will never run. Your object will be full of garbage data and its internal state will be broken.

So when you type the `new` keyword, the C++ compiler secretly translates that into this two step process: Allocate raw, unintialized memory (exaclty what `malloc()` does) , and then call the constuctor on that specific memory address.
```cpp
UartDriver* driver = new UartDriver(115200);
```
```cpp
// STEP 1: Allocate raw, uninitialized memory (exactly what malloc does)
void* rawMemory = operator new(sizeof(UartDriver)); 

// STEP 2: Call the constructor on that specific memory address
// (You cannot type this in standard code; the compiler does it for you)
UartDriver* driver = static_cast<UartDriver*>(rawMemory);
driver->UartDriver::UartDriver(115200); 
```
Note: You cannot type this in standard code; the compiler does it for you.

Note: Because `new` is a language operator (not just a library function like `malloc()`), it is fully type-safe, and it automatically calculates `sizeof()` and automatically casts the returning `void*` pointer to the correct type.

**The Performance Penalty**

The Cherno emphasizes that `new` is slow. when you allocate on the stack, the CPU simply subtracts a value from the Stack Pointer Register and It takes exactly 1 clock cycle.

When you call `new`, you are asking the OS for a favor.
1. **Context Switch:** The Program pauses and hands control to the OS Kernel.
2. **Free List Travesal:** The OS Memory manager searches a massive linked list (The "Free List") looking for a contiguous block of RAM that is large enough to fit your object.
3. **Fragmentation:** If your Heap is highly fragmented (lots of tiny allocation and deletion), this search can take hundreds or thousands of clock cycle.
4. **BookKeeping:** The OS records the size of your allocation in a hidden header right before your pointer, so it knows how much to free when you call `delete`.

Firmware Takeaway: Never call `new` inside a high speed control loop or hardware interrupt (ISR). Allocate all your necessary objects using `new` once during the system boot-up phase, and then reuse them.

**Advanced Firmware Secret: "Placement New"**

Scenario: What if you want to run a C++ Constructor, but you don't want to use the Heap? what if you want to construct an object ar a specific hardcoded memory address (like a memory-mapped hardware register)?

C++ allows a specialized syntax called Placement New. It bypasses Step 1 (memory allocation) and only performs Step 2 (calling the constructor at a provided address).
```cpp
#include <iostream>
#include <new>

class UartDriver {
public:
    UartDriver(int baud) {
        std::cout << "UART Driver Baud rate SET: " << baud << std::endl;
    }
    
    ~UartDriver() {
        std::cout << "UART Driver Instance will now be destroyed!" << std::endl;
    }
};

int main(){
    std::cout << "========== Topic: NEW and DELETE Keyword in C++ ==========" << std::endl;

    // EXAMPLE: Placement New example for firmware based implementation
    // Imagine 0x40004400 is the hardcoded memory address of the USART2 Peripheral
    std::cout << "Advanced firmware Implementation Example using Placement New in C++" << std::endl;
    
    void* hardware_register_address = (void*)0x40004400;

    // Tell C++: "Don't allocate memory. just run the UartDriver constructor directly in tip of this exact memory address"
    UartDriver* usart2 = new (hardware_register_address) UartDriver(115200);

    // NoteL you NEVER call delete on an object created with placement New, because you didn't allocate the memory from the Heap!

    std::cout << "==========================================================" << std::endl;
    std::cin.get();
}
```
```text
Output Log:
========== Topic: NEW and DELETE Keyword in C++ ==========
Advanced firmware Implementation Example using Placement New in C++
UART Driver Baud rate SET: 115200
==========================================================
```
This is how modern embedded C++ frameworks (like Mbed OS or Modern C++ HALs) map object oriented classes directly over baremetal silicon registers!.

## The `this` Keyword in C++
