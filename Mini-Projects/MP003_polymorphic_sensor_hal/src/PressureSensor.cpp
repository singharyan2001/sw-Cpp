// Pressure Sensor Class
#include <iostream>

#include "PressureSensor.h"

PressureSensor::PressureSensor() : i2c_address(0x57), i2c_bus_speed(100'000), sensor_status(SENSOR_OK) {
    std::cout << "PRESSURE SENSOR OBJECT INSTANTIATED" << std::endl;
}

PressureSensor::PressureSensor(uint8_t address, uint32_t bus_speed) : i2c_address(address), i2c_bus_speed(bus_speed), sensor_status(SENSOR_OK) {
    std::cout << "PRESSURE SENSOR OBJECT INSTANTIATED >> ADDRESS: " << (int)address << ", BUS SPEED: " << bus_speed << ", and with status: " << sensor_status << std::endl;
}

bool PressureSensor::init() {
    std::cout << "I2C Pressure Sensor Initialized" << std::endl;
    sensor_status = SENSOR_OK;
    return true;
}

Status PressureSensor::getStatus() const {
    return sensor_status;
}

float PressureSensor::readValue() {
    return 97420;
}
