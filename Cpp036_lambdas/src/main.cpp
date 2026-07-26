// Topic: Lambdas in C++
#include <iostream>
#include <vector>
#include <algorithm> // For std::count_if
#include <functional>   // Required for modern callbacks

/* Capture Modes Explained
 * You can capture variables by Value (making a copy) or by Reference (pointing to the original).
 * [x]: Capture variable x by value (read-only copy).
 * [&x]: Capture variable x by reference (can modify the original).
 * [&]: Capture everything in the surrounding scope by reference.
 * [this]: Capture the current class instance pointer (allows the lambda to call other class methods).
 * [=]: Capture everything in the surrounding scope by value.
*/

// ==========================================
// LOW-LEVEL HAL 
// ==========================================
class GpioDriver {
private:
    // std::function replaces the old void(*)()
    std::function<void()> m_callback = nullptr;

public:
    void attachInterruptHandler(std::function<void()> callback){
        m_callback = callback;
    }

    void simulateHardwareTrigger(){
        if(m_callback){
            m_callback();
        }
    }
};

// ==========================================
// HIGH-LEVEL APPLICATION
// ==========================================
class Motor{
public:
    void cutPower(){
        std::cout << "[MOTOR] Power cut. Rotors stopped.\n";
    }
};


int main(){
    std::cout << "========== TOPIC: Lambdas in C++ ==========\n";
    
    // Define the lambda and store it in a variable
    auto printWarning = [](int error_code){
        std::cout << "[WARNING] Error code: " << error_code << std::endl;
    };

    // Call it exactly lije a normal function
    printWarning(404);

    /* EXAMPLE 02: The Superpower: The Capture Block */
    std::vector<int> sensor_readings = { 10, 25, 40, 55, 80 };

    // A Local variable set by the user or a config file
    int alarm_threshold = 50;

    // THE LAMBDA: Capture the local variable alarm_threshold by value
    // We put 'alarm_threshold' in the capture block to use it inside the lambda
    auto isDanger = [alarm_threshold](int reading){
        return reading > alarm_threshold;
    };

    // std::count_if() automatically passes each element of the vector into the lambda
    int danger_count = std::count_if(sensor_readings.begin(), sensor_readings.end(), isDanger);

    std::cout << "Readings above " << alarm_threshold << ": " << danger_count << std::endl;

    /* EXAMPLE 03: Lambdas as Callbacks */
    GpioDriver estopPin;
    Motor leftRotor;

    // We write the callback INLINE
    // We capture '&leftRotor' by reference so the lambda can command the motor!
    estopPin.attachInterruptHandler([&leftRotor]() {
        std::cout << "[INTERRUPT] E-Stop button pressed!\n";
        leftRotor.cutPower();
    });

    std::cout << "Drone Flying...\n";

    // Hardware voltage changes...
    estopPin.simulateHardwareTrigger();

    std::cout << "=====================================================\n";
}