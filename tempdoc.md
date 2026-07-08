# Temporary Document

## Arrays in C++
An array is a collection of elements of the same data type stored in contiguous (sequential) memory locations. In systems and firmware programming, arrays are the fundamental building blocks for buffers, memory maps, and lookup tables.

#### Raw Arrays vs. C++11 Standard Arrays (`std::array`)
In C++, you have two primary ways to declare a static (fixed-size) array on the Stack:
```cpp
//C-Style Raw Array:
int raw[5] = {1, 2, 3, 4, 5};
//Modern Standard Array:
std::array<int, 5> modern = {1, 2, 3, 4, 5}; // (Requires #include <array>)
```
Memory Layout Equivalence: Mechanically, both types look identical in RAM. They are both contiguous blocks of memory allocated directly on the Stack.
`Memory Address offset = Base Address + (Index * Size of element)`

A `std::array` has zero runtime overhead. It is a header-only wrapper around a raw array. It is the ultimate example of a C++ "Zero-Cost Abstraction."

The "Pointer Decay" Problem: In C, when you pass a raw array to a function, it silently "decays" into a raw pointer to its first element. You lose all type-safety and size information.
```cpp
// In a raw array, size information is lost upon passing!
void processRaw(int* arr) {
    // How big is arr? We have no idea unless we pass a separate size argument.
    // sizeof(arr) returns 8 (the size of a 64-bit pointer address), NOT the array size!
}

// In std::array, the size is baked directly into the TYPE signature!
void processModern(const std::array<int, 5>& arr) {
    // The compiler guarantees this array has exactly 5 elements.
    size_t length = arr.size(); // Compile-time query!
}
```

#### Stack vs. Heap Allocation
Where you allocate your array determines its speed, safety, and lifespan.
```cpp
// 1. STACK ALLOCATION
void stackExample() {
    std::array<int, 100> stackArr; // Created instantly via Stack Pointer math.
} // Automatically destroyed when stackExample exits. Zero chance of a leak.

// 2. HEAP ALLOCATION
void heapExample() {
    // Requesting 400 bytes from the Operating System / Heap allocator
    int* heapArr = new int[100]; 
    
    // Use it...
    heapArr[0] = 42;

    // MANDATORY manual cleanup
    delete[] heapArr; 
} // If you forget delete[], you leak 400 bytes of RAM.
```
Performance & Memory Considerations:
- The Stack: Allocation is incredibly fast (literally 1 CPU clock cycle to adjust the Stack Pointer). However, Stack memory is tiny (often 2KB on a microcontroller, 8MB on standard Linux). Large arrays on the stack cause a Stack Overflow.
- The Heap: Allocation is slow (requires searching the OS "Free List" for contiguous space). Repeated heap allocations can cause Heap Fragmentation, which is a death sentence for long-running firmware.

#### Size Tracking & Safety
Because raw arrays decay to pointers, you must manually track their size. If you get it wrong, you end up with memory corruption.
1. The Traditional C++ Way to Pass Raw Arrays: If you must use raw arrays, C++ allows you to use Templates to capture the array size at compile-time by passing it by reference.
```cpp
template <size_t N>
void printRawArray(int (&arr)[N]) {
    // N is resolved at compile time based on the array passed!
    for (size_t i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
}

int main() {
    int myBuffer[12] = {0};
    printRawArray(myBuffer); // Compiler automatically deduces N = 12
}
```
2. The Modern Standard Way (`std::array`): It keeps track of its own size safely and cleanly through its member functions.
```cpp
std::array<int, 4> myBank = {10, 20, 30, 40};
std::cout << "Elements: " << myBank.size() << "\n"; // Returns 4
```

#### Memory Access Violations & Bounds Checking
What happens when you write to an index that doesn't exist?
```cpp
int raw[3] = {1, 2, 3};
raw[5] = 999; // CRITICAL BUG: Out-of-bounds write!
```
In C and C++, writing `raw[5]` does not trigger a compiler error. The CPU will simply calculate the memory address at `raw + (5 * 4)` and write `999` there.
- If that memory address belongs to another variable in your program, that variable is now silently corrupted.
- If that memory address belongs to an invalid memory space, your program crashes with a Segmentation Fault (Memory Access Violation).

How `std::array` Gives You the Best of Both Worlds: `std::array` provides two ways to access elements:
```cpp
std::array<int, 3> modern = {1, 2, 3};

// Method 1: Subscript Operator []
modern[5] = 999; 
// Fast! No bounds checking is performed at runtime.
// WARNING: Just as dangerous as a raw array if you go out of bounds.

// Method 2: .at() Method
try {
    modern.at(5) = 999;
} catch (const std::out_of_range& e) {
    std::cout << "Safely caught error: " << e.what() << "\n";
}
// Safe! Performs runtime bounds-checking. 
// If out of bounds, it throws a standard C++ exception.
```

#### Why Firmware Engineers Love `std::array`
For embedded developers, `std::array` is the absolute gold standard:
1. No Heap Dependency: It does not use `new` or `malloc()`. It compiles down to standard stack/static arrays.
2. Deterministic Timing: Because it is stack-allocated and static, operations take a fixed, predictable number of clock cycles (no random OS allocator stalls).
3. Type Safety: You cannot accidentally pass a `std::array<int, 5>` to a function expecting a `std::array<int, 10>`. The compiler will block the build, catching bugs before they ever reach your hardware.

---

## String in C++
- Discussed about pointers, arrays, and Characters for strings in C++
- Discussed about char, ASCII and character encoding schemes.
- Discussed how a string looks like in memory.
- Discussed about null termination character in char array / string.
- Discussed about double quotes and single quotes used with characters and how the meaning changes.
- Discussed about standard lib sting and basic string, and how std::string is a templated version of the basic string class, whcih is templated with char. Its a template specialization of the basic string class with char as the template parameter, which means char is the underlying data type of each element.
- Discussed about adding or concatenating two strings.
- Discussed about the methods avaialble in std::string class.
- Discussed about passing strings in functions as arguments.
- Summary: The Cherno breaks down the underlying mechanics of string representation in memory, explaining character encoding, pointers, and the importance of null termination. Practical demonstrations show how to declare, manipulate, and utilize the std::string class within C++ projects.

### Personal Notes
In C, a string is simply a pointer to a null-terminated array of characters. In C++, we have the powerful `std::string` class. While `std::string` makes text manipulation incredibly easy, it hides dynamic memory allocations that can cause performance issues in real-time systems.

#### The Low-Level Reality: C-Style Strings
At the hardware level, there is no such thing as a "string." There are only contiguous bytes of ASCII values in memory.
- Character vs. String Literal:
    - Single Quotes (`'A'`): Represents a single char literal. It consumes exactly 1 byte of memory and holds the integer ASCII value (e.g., 65).
    - Double Quotes (`"A"`): Represents a String Literal (a read-only C-style string). It is placed in the Flash/ROM memory segment. It automatically appends a null terminator `'\0'` (integer value 0) to mark the end of the text.
```text
Size of String Literal ("A") = 2 Bytes ('A' and '\0')
```
```cpp
const char* rawString = "Hello"; 
// In Flash memory, this looks like:
// [ 'H' ][ 'e' ][ 'l' ][ 'l' ][ 'o' ][ '\0' ]
```

#### Modern C++ Strings (std::string)
`std::string` is not a primitive type; it is a complex container class.

The Template specialization: Under the hood, std::string is actually a template specialization of a wider class called std::basic_string. The standard library defines it like this:
```cpp
using string = std::basic_string<char>;
```
If you needed to support 16-bit characters (like UTF-16 for international emoji support), C++ has other specializations:
1. `std::wstring` (uses `wchar_t`)
2. `std::u16string` (uses `char16_t`)
3. `std::u32string` (uses `char32_t`)

For standard systems and console logs, we strictly use `std::string` (which uses standard 8-bit char elements).

#### Memory Allocation: Stack vs. Heap (SSO)
Why is `std::string` safe? Because it manages its own memory dynamically. If you append text to a string, it will automatically request more memory from the Heap, copy the data, and free the old memory.

The Performance Cost: Because it uses the Heap, standard string manipulation can cause Heap Fragmentation and latency. To combat this, modern compilers implement SSO (Small String Optimization).
- Small Strings (usually 15 or 22 characters): The `std::string` object has a small internal buffer on the Stack. If your string is short, it stores the text directly on the Stack, causing zero heap allocation overhead.
- Large Strings (longer than the buffer): The object requests memory from the Heap and stores a pointer to it.
```cpp
void stringMemoryExample() {
    // 1. Fits within SSO limit (No Heap allocation, super fast)
    std::string shortStr = "Short"; 

    // 2. Exceeds SSO limit (Triggers a dynamic malloc-style allocation on the Heap!)
    std::string longStr = "This string is intentionally made very long to exceed the compiler SSO limit"; 
}
```

#### String Operations & Concatenation
In C, you cannot add two string literals together because they are pointers:
```cpp
// COMPILER ERROR: You cannot add two pointers!
// const char* both = "Hello " + "World"; 
```
In C++, the `std::string` class overloads the + and += operators to allow clean, intuitive concatenation.
```cpp
#include <string>

void concatExample() {
    std::string part1 = "Hello ";
    std::string part2 = "World";
    
    // Uses overloaded operator+
    std::string full = part1 + part2; 
    
    // Modifies memory dynamically
    full += "!"; 
}
```

#### Passing Strings to Functions (Best Practices)
This is one of the most critical habits for a C++ developer. How you pass a string to a function determines whether your program copies memory or runs at native speeds.
```cpp
// ANTI-PATTERN: PASS-BY-VALUE
// DANGER: This makes a complete, deep copy of the string.
// If the string is long, this triggers a slow Heap allocation!
void printLog(std::string s) {
    std::cout << s << "\n";
}

// TRADITIONAL PATTERN: PASS-BY-CONST-REFERENCE
// GOOD: Passes only the memory address (8 bytes). 
// 'const' guarantees the function won't modify your string. Zero copy cost!
void printLog(const std::string& s) {
    std::cout << s << "\n";
}
```

**The Modern C++17 Way: `std::string_view`**
What if you want to pass a raw C-style string literal ("Hello") to your function?
If your function expects `const std::string&`, the compiler has to implicitly convert your raw string into a temporary `std::string` object, which might trigger a heap allocation!

C++17 introduced `std::string_view`. It is a lightweight, read-only wrapper that contains only a pointer to the character array and a size integer (usually 16 bytes total). It never allocates memory.

```cpp
#include <string_view>

// EXCELLENT: Accept both C-strings and C++ strings with absolute zero allocation cost!
void printLog(std::string_view s) {
    std::cout << s << "\n";
}

int main() {
    // 1. Works with raw literal (No allocation)
    printLog("Raw C-String Literal"); 
    
    // 2. Works with C++ std::string (No allocation)
    std::string cppString = "Modern C++ String";
    printLog(cppString); 
}
```

#### Common String Methods
`std::string` provides a rich API that replaces raw pointer arithmetic:
```cpp
std::string text = "Firmware Engine";

// 1. Query properties
size_t len = text.size();     // Returns the length of the string (excluding null terminator)
bool empty = text.empty();    // Returns true if length is 0

// 2. Searching
size_t pos = text.find("ware"); // Returns the starting index (4) or std::string::npos if not found

// 3. Substrings
std::string sub = text.substr(0, 8); // Returns "Firmware" (creates a new string)

// 4. Accessing raw C-style pointer
const char* rawPtr = text.c_str(); // Returns the underlying const char* null-terminated pointer.
                                   // Crucial when passing C++ strings to legacy C driver libraries!
```

---

## String Literals in C++
- Discussed what is a string literal and double quotes.
- Discussed about string literals which are stored in the read only memory section.
- Discussed about `wchar_t*` & L, `char16_t` & u, and `char32_t` & U types and with pointer usage - depends with charater encoding schemes.
- Discussed about `std::string_literals`
- Discussed about memory of string literals.
- Discussed the assembly generated code - which describes how the `name` variable storing a string literal is copied from RAM to the name variable.
- Summary: The Cherno explores how character arrays are stored in memory and explains the critical role of the null termination character. The session covers compiler settings for viewing assembly output and differences between char, wchar_t, and modern UTF types.
