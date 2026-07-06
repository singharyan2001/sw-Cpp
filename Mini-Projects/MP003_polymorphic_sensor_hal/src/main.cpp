// MINI-PROJECT 003: POLYMORPHIC SENSOR HAL
#include <iostream>
#include <stdint.h>

#include "ISensor.h"
#include "TempSensor.h"
#include "PressureSensor.h"

/* TESTING
 * 1 >> Enable Code
 * 0 >> Disable Code
*/
#define TESTING             0

#define DEPLOYED_SENSORS    2

int main(){
    std::cout << "========== TOPIC: POLYMORPHIC SENSOR HAL ==========" << std::endl;
    TempSensor aht20(0x01, 400'000);
    PressureSensor bmp585(0x02, 400'000);

#if TESTING
    aht20.init();
    float temp_data = aht20.readValue();
    Status temp_sensor_state = aht20.getStatus();

    std::cout << "AHT20 Temperature Data: " << temp_data << "*C" << std::endl;
    std::cout << "AHT20 Sensor Status: " << temp_sensor_state << std::endl;

    bmp585.init();
    float pressure_data = bmp585.readValue();
    Status pressure_sensor_state = bmp585.getStatus();

    std::cout << "BMP585 Pressure Data: " << pressure_data << " Pa" << std::endl;
    std::cout << "BMP585 Sensor Status: " << pressure_sensor_state << std::endl;
#else
    ISensor* sensors[DEPLOYED_SENSORS] = {&aht20, &bmp585};
    for(int i=0; i < DEPLOYED_SENSORS; i++){
        sensors[i]->init();
    }
    std::cout << "ALL SENSORS INITALIZED" << std::endl;

    int status = 0;
    float data = 0.0f;
    for(int x=0; x < DEPLOYED_SENSORS; x++){
        status = sensors[x]->getStatus();
        data = sensors[x]->readValue();

        std::cout << "SENSOR STATUS: " << status << std::endl;
        std::cout << "SENSOR DATA: " << data << std::endl;
    }

#endif
    std::cout << "===================================================" << std::endl;
    std::cin.get();
}