#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include <stdint.h>

#include "ISensor.h"

class TempSensor : public ISensor {
private:
    uint8_t i2c_address;
    uint32_t i2c_bus_speed;
    Status sensor_status;
public:
    TempSensor();
    TempSensor(uint8_t address, uint32_t bus_speed);
    ~TempSensor() = default;
    bool init() override;
    Status getStatus() const override;
    float readValue() override;
};

#endif //TEMPSENSOR_H