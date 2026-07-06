#ifndef REGISTERBANK_H
#define REGISTERBANK_H

#include <iostream>
#include <stdint.h>

class RegisterBank {
private:
    uint32_t registers[4];
    uint32_t dummy_register_fallback;
public:
    RegisterBank();
    ~RegisterBank() = default;
    uint32_t& operator[](int index);                // Note this will return a memory address, therefore you will be able to read/write to this memory address.
    const uint32_t& operator[](int index) const;
};

std::ostream& operator<<(std::ostream& stream, const RegisterBank& bank);

#endif // REGISTERBANK_H