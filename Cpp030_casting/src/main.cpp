// Topic: Casting in C++
#include <iostream>
#include <cstdint>
#include <cstring>
#include <bit> //C20

#define MOCK_UART_HARDWARE  1

// ============= STATIC CAST EXAMPLE ==============

// 2. RTOS Callback Conversion
struct SensorContext{
    int id;
};

// A generic callbacks signature common in FreeRTOS or pthread
void onSensorInterrupt(void* raw_context) {
    // We KNOW this void* is actually a SensorContext*
    // static_cast safely bridges it back at compile-time.
    SensorContext* ctx = static_cast<SensorContext*>(raw_context);
    std::cout << "Interrupt on sensor: " << ctx->id << std::endl;
}

// ========== REINTERPRET CAST EXAMPLE ============

struct UartRegisters{
    uint32_t STATUS;
    uint32_t DATA;
};

// TYPE PUNNING EXAMPLE
void parseNetworkPayload(uint8_t* payload, size_t length){
    // DANGER: Strict Aliasing Violation! Might Break on High Optimimization (-O3)
    // float temp = *reinterpret_cast<float*>(payload);

    // Safe C++11 Way: memcpy
    float safe_temp;
    std::memcpy(&safe_temp, payload, length);

    // Safe C++20 Way: std::bit_cast (The Ultimate Modern Standard)
    // float modern_temp = std::bit_cast<float>(*payload);
}

// ========== CONST CAST EXAMPLE ============

// A leagcy C Library Function that you can't edit.
// It doesn't modify the string, but the author forgot to write const char*
void legacy_c_print(char* text){
    std::cout << text << std::endl;
}

void my_modern_cpp_print(const std::string& msg){
    // legacy_c_print(msg.c_str()); // COMPILER ERROR: Loses const qualifier
    // Therefore, we strip the const away just to satisfy the compiler, knowing the C function is safe.
    legacy_c_print(const_cast<char*>(msg.c_str()));
}

// ========== DYNAMIC CAST EXAMPLE ============
class Vehicle {
public:
    // A Virtual destructor makes the class polymorphic!
    virtual ~Vehicle() = default;
};

class Car : public Vehicle {};

class Truck : public Vehicle {
public:
    void loadCargo(){
        std::cout << "Loading heavy cargo into the truck!\n";
    }
};


// ========== MAIN FUNCTION ============

int main(){
    std::cout << "========== Topic: Casting in C++ ==========\n";

    /* Static Cast Example */
    // 1. Primitive Conversion
    float battery_voltage = 12.6f;
    int truncated_volts = static_cast<int>(battery_voltage);    // Safe, drops the .6
    
    SensorContext data = {15};
    void* ptr = &data;
    onSensorInterrupt(ptr);

    /* Reinterpret Cast Example */
    #if MOCK_UART_HARDWARE
        UartRegisters mock_uart_hardware;
        UartRegisters* uart1 = &mock_uart_hardware;
        std::cout << "UART1 Physical BASE ADDRESS: " << std::hex << uart1 << std::endl;
        uart1->DATA = 0xDEADBEEF; // We are now writting directly to hardware!
        uint32_t data_value = static_cast<uint32_t>(uart1->DATA);
        std::cout << "Value at DATA Register of the UART Peripheral: 0x" << data_value << std::endl;
    #else
        // The Datasheet says UART1 starts at hardware address 0x40011000
        const uint32_t UART1_BASE_ADDR = 0x40011000;
        // Force a C++ pointer to point to raw physical silicon
        UartRegisters* uart1 = reinterpret_cast<UartRegisters*>(UART1_BASE_ADDR);
        std::cout << "UART1 Physical BASE ADDRESS: " << std::hex << uart1 << std::dec << std::endl;
    #endif

    /* Const Cast Example */
    const std::string name = "Cristiano Ronaldo";
    my_modern_cpp_print(name);

    /* Dynamic Cast Example - Safe Downcasting */
    // We have a Base Pointer, but it actually holds a Truck
    Vehicle* myVehicle = new Truck();

    // We try to safely "downcast" it to a Truck pointer
    Truck* myTruck = dynamic_cast<Truck*>(myVehicle);

    // ALWAYS check if dynamic_cast succeeded before using the pointer!
    if(myTruck){
        std::cout << "Successfully verified it is a Truck.\n";
        myTruck->loadCargo();   // Safe to call derived methods
    }
    else{
        std::cout << "Cast failed! This is not a Truck.\n";
    }

    // myVehicle = dynamic_cast<Vehicle*>(myTruck); // Upcasting is always safe, but dynamic_cast is not needed here.
    myVehicle = myTruck; // Upcasting is always safe, but dynamic_cast is not needed here.
    if(myVehicle){
        std::cout << "myVehicle is still valid after the cast.\n";
    }

    // What if we try to cast it to a Car?
    // This will fail, because the object is actually a Truck, not a Car.
    Car* myCar = dynamic_cast<Car*>(myVehicle);
    if(!myCar){
        std::cout << "Correctly failed to cast a Truck into a Car. Returns nullptr.\n";
    }
    else{
        std::cout << "This should never happen!\n";
    }

    delete myVehicle;

    std::cout << "===========================================\n";
    std::cin.get();
}