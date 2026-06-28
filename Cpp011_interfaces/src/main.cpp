// Interfaces in C++

#include <iostream>
#include <stdint.h>

// The 'I' prefix is a naming convection meaning "Interface"
class ICommBus {
public:
    // pure Virtual Function: "I will not write this code. You must."
    virtual void transmit(uint8_t data) = 0;

    // Always include a virtual destructor in an Interface!
    virtual ~ICommBus() = default;
};

// Concrete class that implements the ICommBus interface
class I2CBus : public ICommBus {
public:
    // Concrete implementation of the ICommBus Interface
    void transmit(uint8_t data) override {
        std::cout << "Transmit API Implemented/Overriden in I2CBus Class" << std::endl;
        std::cout << "Data Transmitted: 0x" << std::hex << (uint)data << std::endl;
    }
};

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
    I2CBus mySensorBus;     //Instantiate - SUCCESS
    mySensorBus.transmit(0x23);

    FastI2CBus myFastBus;   //Instantiate - SUCCESS
    myFastBus.setFastMode();
    // The above instance automatically uses the transmit() implementation which it inherited from the parent I2CBus Cass
    myFastBus.transmit(0xff);

    std::cout << "==================================" << std::endl;
    std::cin.get();
}