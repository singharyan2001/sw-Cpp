// TOPIC: move Semantics in C++
#include <iostream>
#include <cstdint>
#include <cstring>
#include <memory>

class String{
private:
    char* m_data;
    uint32_t m_size;
public:
    // Constructors
    String() = default;

    String(const char* string){
        printf("Created!\n");
        m_size = strlen(string);
        m_data = new char[m_size];
        memcpy(m_data, string, m_size);
    }

    // Copy Constructor
    String(const String& other){
        printf("Copied!\n");
        m_size = other.m_size;
        m_data = new char[m_size];
        memcpy(m_data, other.m_data, m_size);
    }

    // Move Constructor
    String(String&& other){
        printf("Moved!\n");
        // So here, we have kind of done a shallow copy
        m_size = other.m_size;
        m_data = other.m_data;

        // Turning the other object into a hollow object
        other.m_size = 0;
        other.m_data = nullptr;
    }

    String& operator=(String&& other) {
        printf("Moved!\n");

        if(this != &other){
            // Since we are moving data from another object to an already existing object
            // we would first need to delete the current data and then only moved it to dest object
            // or else there would be a resource leak
            delete[] m_data;

            // So now, we have done a shallow copy now
            m_size = other.m_size;
            m_data = other.m_data;

            // Turning the other object into a hollow object
            other.m_size = 0;
            other.m_data = nullptr;
        }
        return *this;
    }

    // Destructors
    ~String(){
        printf("Destroyed!\n");
        delete m_data;
    }

    void Print(){
        for(uint32_t i = 0; i < m_size ; i++){
            printf("%c", m_data[i]);
        }
        printf("\n");
    }
};

class Entity{
private:
    String m_name;
public:
    Entity(const String& name) : m_name(name) {}

    // Actual here, you would prefer to use std::move, rather than (sting&&)
    // Entity(String&& name) : m_name((String&&)name) {}
    Entity(String&& name) : m_name(std::move(name)) {}

    void PrintName(){
        m_name.Print();
    }
};

// EXAMPLE: EMBEDDED USE CASE: DMA PLAYLOAD [ZERO-DEEP COPY NEEDS TO BE PERFORMED]
class DmaPayload {
private:
    uint8_t* m_buffer;
    size_t m_size;
public:
    // 1. Standard Constructor
    DmaPayload(size_t size) : m_size(size) {
        m_buffer = new uint8_t[m_size];
        std::cout << "[ALLOC] Allocated: " << m_size << " bytes on Heap" << std::endl;
    }

    // 2. Destructor
    ~DmaPayload() {
        delete[] m_buffer;
        std::cout << "[FREE] Destroyed payload." << std::endl;
    }

    // 3. Copy Constructor (Slow Deep Copy)
    DmaPayload(const DmaPayload& other) : m_size(other.m_size) {
        m_buffer = new uint8_t[m_size];
        memcpy(m_buffer, other.m_buffer, m_size);
        std::cout << "[COPY] Performed slow deep copy." << std::endl;
    }

    // 4. Move Constructor (Zero-Copy)
    // 'noexcept' tells the compiler this operation cannot throw errors, unlocking further optimizations
    DmaPayload(DmaPayload&& other) noexcept 
        : m_size(other.m_size), m_buffer(other.m_buffer)    // Steal the pointers/values
    {
        std::cout << "[MOVE CONSTRUCTOR] Stole memory pointer!" << std::endl;
        
        // Neutralize the source object so its destructor doesn't kill our stolen memory!
        other.m_buffer = nullptr;
        other.m_size = 0;
    }

    // 5. Move Assignment Operator
    DmaPayload& operator=(DmaPayload&& other) noexcept {
        std::cout << "[MOVE ASSIGNMENT] Overwriting existing object" << std::endl;

        // WARNING: Self-Assignment Check!
        // What if someone types: myPayload = std::move(myPayload);
        // If we didn't check for this, we would delete our own memory, then try to steal from it.
        if(this != &other){
            // A. Clean up our current memory
            delete[] m_buffer;
            
            // B. Steal the data
            m_size = other.m_size;
            m_buffer = other.m_buffer;
            
            // C. Neutralize the source
            other.m_buffer = nullptr;
            other.m_size = 0;
        }
        return *this;
    }
};

// EXAMPLE: The Modern Approach: Smart Pointers and The "Rule of Zero"
class SmartDmaPayload {
private:
    // The Smart Pointer automatically deletes memory and neutralizes itself on move!
    std::unique_ptr<uint8_t[]> m_buffer;
    size_t m_size;
public:
    // Only the Constructor is needed!
    SmartDmaPayload(size_t size) : m_size(size) {
        m_buffer = std::make_unique<uint8_t[]>(m_size);
        std::cout << "[ALLOC] Smart DMA payload created.\n";
    }
    // RULE OF ZERO: No Destructor, No Copy logic, No Move logic required!
    // The compiler automatically figures out how to move unique_ptrs.

    // Overload the [] Operator to forward the Index to the smart pointer
    uint8_t& operator[](size_t index){
        // Bound Checks!
        if(index >= m_size){
            std::cout << "[ERROR[ OUT OF BOUNDS ACCESS - RETURNING WITH LAST ELEMENT INDEX\n";
            return m_buffer[m_size];
        }
        return m_buffer[index];
    }
};


int main(){
    std::cout << "============ TOPIC: move Semantics in C++ =============\n";
    
    // Entity entity(String("Aryan"));
    Entity entity("Aryan"); // Created, Moved, Destroyed (stolen victim)
    std::cout << "entity: ";
    entity.PrintName(); // printed name

    String src_string = "Old Trafford"; // Created
    std::cout << "src_string: ";
    src_string.Print();
    
    // This does a copy operation behind the scenes.
    String new_src_string = src_string; // Copied
    std::cout << "src_string: ";
    src_string.Print();
    std::cout << "new_src_string: ";
    new_src_string.Print();
    
    // This steals the resources of src_string to mv_src_string
    String mv2_src_string(std::move(new_src_string));   // Moved // Use of std::move
    std::cout << "new_src_string: ";
    new_src_string.Print();
    std::cout << "mv2_src_string: ";
    mv2_src_string.Print();

    // A new object is initialized and the other moves the string to this object
    // therefore no new creations or copy and so on.
    String mv_src_string = std::move(src_string);   // Moved // Use of move assignment operator
    std::cout << "src_string: ";
    src_string.Print();
    std::cout << "mv_src_string: ";
    mv_src_string.Print();

    // Then at the end of program, 5 objects get destroyed
    // i.e entity, src string, new_src_string, mv2_src_string, and mv_src_string.
    std::cout << "=======================================================\n";

    std::cout << "====== Embedded Firmware Example: Heavy DMAPAYLOAD with Zero-Copy ======\n";

    std::cout << "------ Creating Payload A ------\n";
    DmaPayload PayloadA(1024);  // Allocate 1024 bytes

    std::cout << "\n------ Copying to Payload B (slow) ------\n";
    // PayloadA is an LValue (persistent). This triggers the Slow Copy Constructor
    DmaPayload PayloadB = PayloadA;

    std::cout << "\n------ Moving to Payload C (Fast) ------\n";
    // We are completely done with payloadA. We cast it to a temporary using std::move
    // This triggers the Zero-Copy Move Constructor
    DmaPayload PayloadC = std::move(PayloadA);

    // IMPORTANT: At this exact moment, PayloadA is just an empty shell.
    // Its internal pointer is nullptr. You must NEVER try to read from payloadA again!

    std::cout << "\n------ Reassigning Payload B ------\n";
    DmaPayload tempPayload(512);

    // PayloadB already exists. We use std::move to trigger the Move Assignment Operator!
    PayloadB = std::move(tempPayload);

    std::cout << "\n------ End of Scope Cleanup ------\n";

    std::cout << "====== Embedded Firmware Example: The Modern Approach with Heavy DMAPAYLOAD with Zero-Copy ======\n";

    SmartDmaPayload PayloadX(256);

    // COMPILER ERROR!
    // unique_ptr cannot be copied, so the compiler safely prevents accidental Deep Copies!
    // SmartDmaPayload PayloadY = PayloadX;

    // SUCCESS! Fast, Zero-Copy transfer!
    // The Compiler automatically moves the unique_ptr and copies the size variable.
    SmartDmaPayload PayloadZ = std::move(PayloadX);

    // Write Complete Buffer
    for(int i=0; i < 20; i++){
        PayloadZ[i] = 0xAA;
    }

    // Read Complete Buffer
    for(int x=0; x < 20; x++){
        uint8_t data = PayloadZ[x];
        std::cout << "Data: 0x" << std::hex << static_cast<int>(data) << std::dec << std::endl;
    }

    // unique_ptr automatically cleans up the 2048 bytes!

    std::cout << "==================================================================================================\n";
    std::cin.get();
}