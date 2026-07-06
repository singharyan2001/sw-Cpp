#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include <stdint.h>

#include "ISensor.h"

class PressureSensor : public ISensor {
private:
    uint8_t i2c_address;
    uint32_t i2c_bus_speed;
    Status sensor_status;
public:
    PressureSensor();
    PressureSensor(uint8_t address, uint32_t bus_speed);
    ~PressureSensor() = default;
    bool init() override;
    Status getStatus() const override;
    float readValue() override;
};

#endif // PRESSURESENSOR_H