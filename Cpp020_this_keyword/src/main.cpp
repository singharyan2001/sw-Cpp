// This Keyword in C++
#include <iostream>

// The C Way/Style Example
struct Sensor {
    int pin;
};

// You manually pass the pointer to the struct
void Sensor_SetPin(struct Sensor* ptr, int p){
    ptr->pin = p;
    std::cout << "Sensor Pin SET Value: " << ptr->pin << std::endl;
}

// The C++ Way/Style Example
class SensorX {
private:
    int m_pin;
public:
    SensorX() : m_pin(1) {
        std::cout << "[CONSTRUCTOR] SENSOR INSTANCE INITIALIZED WITH DEFAULT PIN 1" << std::endl;
    }

    SensorX(int pin) : m_pin(pin) {
        std::cout << "[CONSTRUCTOR] SENSOR INSTANCE INITIALIZED WITH DEFAULT PIN " << pin << std::endl;
    }

    ~SensorX() {
        std::cout << "[DESTRUCTOR] SENSOR INSTANCE WITH DEFAULT PIN " << m_pin << "- DESTROYED" << std::endl;
    }

    void setPin(int p){
        // 'this' is magically avilable here!
        this->m_pin = p;
        std::cout << "NEW PINS SET: " << this->m_pin << std::endl; 
    }
};

// USE CASE: Name Shadowing
class UartDriver {
private:
    int baud_rate;
public:
    // The Parameter is also named 'baud_rate'
    UartDriver(int baud_rate){
        // baud_rate = baud_rate;       // Wrong! just assigns the parameter to itself.
        this->baud_rate = baud_rate;    // Correct! assigns parameter to object.
    }
};

// USE CASE: Passing "self" to external functions (Hardware callbacks)
// A global function
void register_sensor_callback(class SensorY* s);

class SensorY {
public:
    void init(){
        // Hey Global System, register ME!
        register_sensor_callback(this);
    }

    void onHardwareInterrupt(){
        std::cout << "ISR TRIGGERED & PROCESSED" << std::endl;
    }
};

// USE CASE: Returning References to Self (Method Chaining)
class LedController {
public:
    LedController& turnOn(){
        std::cout << "LED TURNED ON!" << std::endl;
        return *this;   // return the object itself
    }

    LedController& setBrightness(int level){
        std::cout << "LED BRIGHTNESS SET: " << level << std::endl;
        return *this;
    }
};

// delete this example
class TemporaryTask {
public:
    TemporaryTask(){
        std::cout << "TASK CREATED - NOW RUNNING" << std::endl;
    }
    void finishTask() {
        // ... do work ...
        
        // Destroy myself!
        delete this; 
    }
};

int main(){
    std::cout << "========== Topic: this Keyword in C++ ==========" << std::endl;

    std::cout << "USE CASE: C Style Example (this keyword does not exist here in C" << std::endl;
    struct Sensor mySensor;
    Sensor_SetPin(&mySensor, 5);

    std::cout << "USE CASE: C++ Style Example (Using this Keyword)" << std::endl;
    SensorX ntc1;
    SensorX ntc2(5);
    ntc1.setPin(5);
    // ^ The compiler secretly translates this into: SensorX::setPin(&ntc1, 5);
    ntc2.setPin(9);
    // ^ The compiler secretly translates this into: SensorX::setPin(&ntc2, 9);

    // EXAMPLE: Passing "self" to external functions (Hardware callbacks)
    std::cout << "USE CASE: Passing self (via this keyword) to external functions Example" << std::endl;
    SensorY testSensor;
    testSensor.init();
    // testSensor.onHardwareInterrupt();

    // EXAMPLE: Returning References to Self (Method Chaining)
    std::cout << "USE CASE: RETURNING REFERENCES to self via this keyword to achieve method chaining in C++" << std::endl;
    LedController led;
    // Method chaining made possible by returning (*this)
    led.turnOn().setBrightness(100);
    
    std::cout << "EXAMPLE: Using delete and this keyword to destroy an object - NOT RECOMMENDED!!" << std::endl;
    TemporaryTask task;
    task.finishTask();

    std::cout << "================================================" << std::endl;
    std::cin.get();

}

void register_sensor_callback(class SensorY* s){
    s->onHardwareInterrupt();
}