// Topic: Copying and copy constructor
#include <iostream>
#include <cstring>

// If ENABLED(1), then the program will crash because of double free. If DISABLED(0), then the program will run fine.
#define BAD_COPY_STRING_EXAMPLE     0

#define BAD_PASS_BY_EXAMPLE         1

class BadString{
private:
    char* m_buffer;
    int m_size;
public:
    BadString(const char* string) {
        m_size = strlen(string);
        m_buffer = new char[m_size + 1]; // Ask Heap for memory and +1 for adding null termination
        memcpy(m_buffer, string, m_size + 1); // Copy text into Heap
    }

    ~BadString(){
        delete[] m_buffer; // Automatically free the memory
    }
};

class GoodString{
private:
    char* m_buffer;
    int m_size;
public:
    GoodString(const char* string){
        m_size = strlen(string);
        m_buffer = new char[m_size + 1];
        memcpy(m_buffer, string, m_size + 1); // destination address, source address, total size
    }

    // The Copy Constructor (deep copy)
    // 'other' is a reference to the object we are copying FROM.
    GoodString(const GoodString& other) : m_size(other.m_size) {
        std::cout << "[Copied String!]" << std::endl;
        // 1. Ask the Heap for BRAND NEW memory just for this copy
        m_buffer = new char[m_size + 1];
        // 2. Copy the actual data bytes from the 'other' buffer into the NEW buffer
        memcpy(m_buffer, other.m_buffer, m_size + 1);
    }

    // VIP PASS: "I allow this specific external function to read my private variables"
    friend std::ostream& operator<<(std::ostream& stream, const GoodString& string);

    ~GoodString(){
        delete[] m_buffer;
    }
};

// Now this external function can legally access string.m_buffer
std::ostream& operator<<(std::ostream& stream, const GoodString& string){
    stream << string.m_buffer;
    return stream;
}

#if BAD_PASS_BY_EXAMPLE
// BAD: This creates a full, slow Deep Copy on the Heap every time you call it!
void printString(GoodString text) {
    std::cout << text << std::endl;
}
#else
// GOOD: Pass-by-Const-Reference. 
// Passes an 8-byte pointer address. The Copy Constructor is NEVER called!
void printString(const GoodString& text) {
    std::cout << text << std::endl;
}
#endif

int main(){
    std::cout << "========== TOPIC: Copying & Copy Constructor in C++ ==========\n";

    #if BAD_COPY_STRING_EXAMPLE
        std::cout << "BAD COPY STRING EXAMPLE\n";
        BadString StringA("Hello");
        // Shallow copy occurs here
        BadString StringB = StringA;
    #else
        std::cout << "GOOD COPY STRING EXAMPLE\n";
        GoodString StringX = "FIFA26";
        // The Compiler sees the `=` and triggers the copy constructor
        GoodString StringZ = StringX;

        std::cout << "StringX: " << StringX << ", StringZ (Copied): " << StringZ << std::endl;

        // Now, StringX is at 0x1000, and StringY is at 0x2000, they now safely delete themeselves! No Crash!
    #endif

    printString(StringX);

    std::cout << "==============================================================\n";
    std::cin.get();
}
// if ENABLED example, then CRASH!!! Segmentation fault because of double free!!
// Else, NO Crash!!