// MINI-PROJECT 005: Generic Ring Buffer
#include <iostream>
#include <string>
#include <vector>

#include "GenericRingBuffer.hpp"

int main(){
    std::cout << "========== MINI-PROJECT: GENERIC RING BUFFER ==========\n";

    std::cout << "== TEST-CASE 01: INTEGER BUFFER ==\n";
    RingBuffer<int> intBuffer(10);
    intBuffer.print_status();
    
    // PUSH Test
    intBuffer.push(10);
    intBuffer.push(20);
    intBuffer.push(30);
    intBuffer.push(40);
    intBuffer.push(50);
    intBuffer.push(60);
    intBuffer.push(70);
    intBuffer.push(80);
    intBuffer.push(90);
    intBuffer.push(100);

    // OVERFLOW CONDITION CHECK
    bool status = intBuffer.push(100);
    std::string result = (status == true) ? "PUSH PASSED - WAS NOT SUPPOSED TO HAPPEN" : "PUSH FAILED - AS EXPECTED, TEST PASSED";
    std::cout << "OVERFLOW PUSH CONDITION CHECK: " << result << std::endl;

    std::cout << std::endl;
    intBuffer.print_status();

    //POP TEST
    int data = 0;
    std::cout << "Data Popped from RIng Buffer: ";
    while(intBuffer.pop(data)){
        std::cout << data << ", ";
    }
    std::cout << std::endl;
    intBuffer.print_status();

    std::cout << "== TEST-CASE 02: STRING RING BUFFER ==\n";
    RingBuffer<std::string> messageBuffer(10);
    messageBuffer.print_status();
    std::cout << "MESSAGE BUFFER PUSH TEST!\n";
    messageBuffer.push("cr7");
    messageBuffer.push("lm10");
    messageBuffer.push("njr11");
    messageBuffer.print_status();

    auto holder = messageBuffer[0];
    std::cout << "[ILLEGAL ACCESS] DIRECT ACCESS OF RING BUFFER via Operator Overloadig the [] operator: " << holder << std::endl;

    std::string info;
    std::cout << "MESSAGE BUFFER POP TEST: ";
    while(messageBuffer.pop(info)){
        std::cout << info << ", ";
    }
    std::cout << std::endl;
    intBuffer.print_status();

    // std::cout << "== TEST-CASE 03: VECTOR RING BUFFER ==\n";

    std::cout << "=======================================================\n";
    std::cin.get();
}