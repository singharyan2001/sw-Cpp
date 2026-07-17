#include <iostream>

#include <vector>
#include <string>

struct WeatherSnapshot {
    float temp;
    float humidity;
};

// Global sensor snapshot
WeatherSnapshot current_reading = {24.5f, 62.0f};

// A function returning a reference to save memory
const WeatherSnapshot& getLatestReading() {
    return current_reading;
}

// The compiler generates a unique internal class for this lambda.
// 'auto' is the only way to capture it.
auto processAdc = [](int raw_value) {
    return (raw_value * 3.3f) / 4095.0f; 
};

int main(){
    std::cout << "================== TOPIC: AUTO KEYWORD IN C++ ==================" << std::endl;

    auto scale = 42;          // Compiler deduces: int
    auto voltage = 3.3f;      // Compiler deduces: float
    auto status = true;       // Compiler deduces: bool

    std::cout << "Scale: " << scale << ", Voltage: " << voltage << ", Status: " << status << std::endl;

    int raw_adc_reading; // DANGER: Uninitialized! Contains garbage data.
    // auto raw_adc_reading; 
    // COMPILER ERROR: Declaration of 'auto' variable requires an initializer

    // TRAP: 'auto' discards const and reference!
    // The compiler deduces: WeatherSnapshot (a plain copy).
    // This physically duplicates current_reading onto the stack at runtime.
    auto copy_reading = getLatestReading(); 

    // FIX: You must explicitly add const and reference to auto!
    // The compiler deduces: const WeatherSnapshot&
    // Zero overhead. Pointing directly to current_reading in memory.
    const auto& ref_reading = getLatestReading();

    /* The Rule of Thumb for Performance:
     * 1. Use auto for basic primitives (int, float, char) and when you explicitly want a copy.
     * 2. Use const auto& by default for any complex object, class, or structure to prevent slow stack-copy loops.
     * 3. Use auto& if you need a writable reference to modify the original object.
    */
    
    std::vector<std::string> log_buffer = {"hello", "there", "!!"};

    // WITHOUT AUTO (Verbose, unreadable, hard to refactor)
    for (std::vector<std::string>::const_iterator it = log_buffer.begin(); it != log_buffer.end(); ++it) {
        // Process log...
        std::cout << *it << std::endl;
    }

    // WITH AUTO (Clean, elegant, refactor-safe)
    for (auto it = log_buffer.begin(); it != log_buffer.end(); ++it) {
        // Process log...
        std::cout << *it << std::endl;
    }

    float x_voltage = processAdc(2048);
    std::cout << x_voltage << std::endl;

    std::cout << "================================================================\n";
    std::cin.get();
}