// RegisterBank.cpp

#include "RegisterBank.h"

RegisterBank::RegisterBank() : registers{0,0,0,0} {
    std::cout << "REGISTER BANK OBJECT INSTANTIATED WITH MEMORY MAP ALLOCATED" << std::endl;
}

uint32_t& RegisterBank::operator[](int index){
    if ( (index < 0) || (index > 3) ){
        std::cout << "[ERROR] OUT OF BOUNDS ACCESS DETECTED" << std::endl;
        return dummy_register_fallback;
    }
    return registers[index];
}

const uint32_t& RegisterBank::operator[](int index) const {
    if( (index < 0) || (index > 3) ){
        std::cout << "[ERROR] OUT OF BOUNDS ACCESS DETECTED" << std::endl;
        return registers[0];
    }
    return registers[index];
}

std::ostream& operator<<(std::ostream& stream, const RegisterBank& bank){
    for(int i=0; i < 4; i++){
        stream << "[REGISTER BANK] Register " << i << " Value: 0x" << std::hex << bank[i] << "\n";
    }
    // Need to handle global stream pollution - because now my stream is in hex mode, so will need to set it back to dec mode.
    // Reset back to decimal so the rest of the program works correctly!
    stream << std::dec;
    return stream;
}