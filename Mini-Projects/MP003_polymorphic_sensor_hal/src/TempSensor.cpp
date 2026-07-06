// Temperature Sensor SubClass
#include <iostream>
#include <TempSensor.h>

TempSensor::TempSensor() : i2c_address(0x40), i2c_bus_speed(100'000), sensor_status(SENSOR_OK) {
    std::cout << "TEMP SENSOR OBJECT INSTANTIATED" << std::endl;
}

TempSensor::TempSensor(uint8_t address, uint32_t bus_speed) : i2c_address(address), i2c_bus_speed(bus_speed), sensor_status(SENSOR_OK) {
    std::cout << "TEMP SENSOR OBJECT INSTANTIATED with ADDRESS: " << (int)address << ", BUS SPEED: " << bus_speed << ", and with status: " << sensor_status << std::endl;
}

bool TempSensor::init() {
    std::cout << "I2C Temperature Sensor Intialized" << std::endl;
    sensor_status = SENSOR_OK;
    return true;
}

Status TempSensor::getStatus() const {
    return sensor_status;
}

float TempSensor::readValue() {
    return 24.04f;
}
