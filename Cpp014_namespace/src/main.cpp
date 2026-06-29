// Namespaces in C++
#include <iostream>
#include <stdint.h>

namespace UART {
    int baud_rate = 115200;
    
    void init(){
        std::cout << "UART Peripheral Initialized" << std::endl;
    }

    void transmit(char data){
        std::cout << "Transmitting data via UART: " << data << std::endl;
    }
}

namespace SPI {
    int clock_speed = 400'000;
    
    void init(){
        std::cout << "SPI Peripheral Initialized" << std::endl;
    }

    void transmit(char& data){
        std::cout << "Transmitting data via SPI: 0x" << std::hex << (int)(unsigned char)data << std::endl;
    }
}


int main(){
    std::cout << "==== Topic: Namespaces in C++ ====" << std::endl;
    char spi_byte = 0xAA;
    UART::init();
    SPI::init();

    UART::transmit('A');
    SPI::transmit(spi_byte);

    std::cout << "==================================" << std::endl;
    std::cin.get();
}