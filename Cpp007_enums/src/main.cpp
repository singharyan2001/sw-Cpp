
#include <iostream>

inline void log_msg(const char* message){
    std::cout << message << std::endl;
}

inline void log_data(const char* message, int& data){
    std::cout << message <<  data << std::endl;
}

// Plain Enum in C++
enum color{
    RED,
    BLUE,
    GREEN,
    ORANAGE
};

// Enum Class in C++
enum class TrafficLights{
    RED,
    BLUE,
    GREEN,
    ORANGE
};

// Creating Enums in Class



int main(){
    log_msg("=========== TOPIC: ENUMS IN C++ ===================");
    int col1 = RED;
    log_data("COLOR ENUM RED: ", col1);

    /* 
     * Removing (int) creates an issue because strongly typed enums do not automatically convert to integers.
     * No Implicit Conversion: Scoped enums prevent accidental mistakes by blocking implicit conversions to integers.
     * Stream Limitation: The std::cout stream does not know how to print a scoped enum directly. It requires a defined text representation or a numeric value.
     * Compiler Error: Without (int), the compiler throws an error because it cannot find a matching << operator for your specific enum type. 
    */
    TrafficLights col2 = TrafficLights::RED;
    std::cout << "Traffic Lights Status: " << (int)col2 << std::endl;

    log_msg("===================================================");
    std::cin.get();
}