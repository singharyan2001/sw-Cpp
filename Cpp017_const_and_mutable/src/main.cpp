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
    std::cout << "========== TOPIC: const in C++ ==========" << std::endl;
    
    // EXAMPLE: Constant and References
    const SensorPayload data = {
        {24.6, 27.3},
        1782835800
    };
    printPayload(data);

    // EXAMPLE: A Pointer to a Constant (data)
    int a = 5;
    int b = 10;
    const int* ptr = &a;    // ptr is a pointer to an integer that is constant.
    ptr = &b;               // OK: You can change WHERE the pointer points.
    //*ptr = 7;             // ERROR: You cannot change the VALUE it points to.

    // EXAMPLE: Constant Pointer
    int* const ptr2 = &a;       // ptr2 is a pointer that is constant to an integer.
    *ptr2 = 7;                  // OK: You can change the data WHERE the pointer is pointing to.
    //ptr2 = &b;                //ERROR: You cannot change WHERE the pointer is pointing to.

    // EXAMPLE: Constant Pointer to a Constant (data)
    const int* const ptr3 = &b; // ptr3 is a pointer that is constant to an integer that is constant.
    //*ptr3 = 20;           // ERROR: You cannot change the VALUE the pointer points to.
    //ptr3 = &a;            // ERROR: You cannot change WHERE the pointer points to.
    // // Completely locked down. You can only read it.

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