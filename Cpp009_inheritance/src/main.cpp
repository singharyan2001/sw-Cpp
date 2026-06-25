#include <iostream>
#include <stdint.h>

class i2c_slave{
private:
    uint8_t  m_slave_address;
    uint32_t m_bus_speed;
    bool m_internal_pull_up_en;
public:
    // Default Constructor
    i2c_slave(){
        m_slave_address = 0x40;         // Default Sensor Address
        m_bus_speed = 100000;           //100 Khz - Standard Bus Speed
        m_internal_pull_up_en = true;   // Internal pull-up enable status
        std::cout << "[CONSTRUCTOR] I2C SENSOR INSTANCE INTIALIZED - WITH PARAMS: " << std::endl;
        std::cout << "DEFAULT I2C SENSOR ADDRESS: 0x" << std::hex << (uint)m_slave_address << std::endl;
        std::cout << "DEFAULT I2C BUS SPEED: " << std::dec << m_bus_speed << " KHz" << std::endl;
        std::cout << "DEFAULT I2C INTERNAL BUS PULL-UP EN: " << m_internal_pull_up_en << std::endl;
    }

    // Constructor with Input Arguments
    i2c_slave(uint8_t slave_addr, uint32_t bus_speed, bool internal_pull_up_en){
        m_slave_address = slave_addr;               // Default Sensor Address
        m_bus_speed = bus_speed;                    //100 Khz - Standard Bus Speed
        m_internal_pull_up_en = internal_pull_up_en;     // Internal pull-up enable status
        std::cout << "[CONSTRUCTOR] I2C SENSOR INSTANCE INTIALIZED - WITH PARAMS: " << std::endl;
        std::cout << "I2C SENSOR ADDRESS: 0x" << std::hex << (uint)slave_addr << std::endl;
        std::cout << "I2C BUS SPEED: " << std::dec << bus_speed << " KHz" << std::endl;
        std::cout << "I2C INTERNAL BUS PULL-UP EN: " << internal_pull_up_en << std::endl;
    }

    // Default Destructor
    ~i2c_slave(){
        std::cout << "[DESTRUCTOR] I2C SENSOR INSTANCE RESOURCES RELEASED" << std::endl;
    }

    // APIs
    void i2c_read(){
        std::cout << "I2C SENSOR READ SUCCESS" << std::endl;
    }
    void i2c_write(){
        std::cout << "I2C SENSOR WRITE SUCCESS" << std::endl;
    }
};

// Inheritance Example below
class sensor_ina260 : public i2c_slave {
private:
    uint16_t m_ina260_calibration_data;
public:
    float voltage, current, power;
    // Default Constructor
    sensor_ina260(){
        std::cout << "[CONSTRUCTOR] INA260 SENSOR INITALIZED" << std::endl;
        m_ina260_calibration_data = 0x0000;
        voltage = 0.00f;
        current = 0.00f;
        power = 0.00f;
    }

    // Default Destructor
    ~sensor_ina260(){
        std::cout << "[DESTRUCTOR] INA260 SENSOR RESOURCES RELEASED" << std::endl;
    }

    void log_sensor_info(){
        std::cout << "INA260 SENSOR - DESIGNED BY TI - USED FOR PWR SENSING APPLICATIONS" << std::endl;
    }

    void fetch_calibration(){
        i2c_read();
        m_ina260_calibration_data = 0x2890;
        std::cout << "INA260 SENSOR CALIBRATION VALUE: 0x2890" << std::endl;
    }
};

int main(){
    // i2c_slave in219;
    i2c_slave ina219(0x44, 400000, true);

    sensor_ina260 charger_power_sensor;
    charger_power_sensor.fetch_calibration();

    std::cout << "SIZE OF I2C_SLAVE CLASS: " << sizeof(i2c_slave) << std::endl;
    std::cout << "SIZE OF SENSOR_INA260 CLASS: " << sizeof(sensor_ina260) << std::endl;
    
    std::cin.get();
}