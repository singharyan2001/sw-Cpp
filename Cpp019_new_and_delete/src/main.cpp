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