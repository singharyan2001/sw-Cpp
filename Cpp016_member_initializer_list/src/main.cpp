// Member Initializer List in C++
#include <iostream>

#define MODERN_PATTERN_CPP  1

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

// reference example for member intializer list
class DataLogger {
private:
    int& rx_buffer; // Reference to an external buffer
public:
    DataLogger(int& buf) : rx_buffer(buf) {
        // Empty Constructor
    }
};

int main(){
    std::cout << "========== TOPIC: Member Intializer List in C++ ==========" << std::endl;
    FlightController fc(9600);
    Sensor i2c(0x40);
    int mem_size = 10;
    DataLogger buffer(mem_size);

    std::cout << "==========================================================" << std::endl;
    std::cin.get();
}