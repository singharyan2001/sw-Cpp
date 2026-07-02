// Operators and Operator Overloading in C++
#include <iostream>

// EXAMPLE: The Core Syntax (Math Operators)
struct Vector3 {
    float x,y,z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // The Overloaded `+` Operator
    // Notice the const reference (fast, safe) and the trailing const (read-only)
    Vector3 operator+(const Vector3& other) const {
        // Return a brand new Vector3 Object with the added values
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // The Overloaded '*' Operator (Scaling by a scalar value)
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
};

// EXAMPLE: The Equality Operator (==)
struct SensorConfig {
    int baud_rate;
    int pin_tx;
    int pin_rx;
    
    // Overloading the '==' operator
    bool operator==(const SensorConfig& other) const{
        return (baud_rate == other.baud_rate) && (pin_tx == other.pin_tx) && (pin_rx == other.pin_rx);
    }
    
    // It's good practice to also overload '!=' when you overload '=='
    bool operator!=(const SensorConfig& other) const{
        return !(*this == other); // Reuse the '==' logic using the 'this' pointer!
    }
};


// EXAMPLE: The Left Shift Operator (<<) and std::cout
// Global Operator Overload
std::ostream& operator<<(std::ostream& stream, const Vector3& v){
    stream << "X: " << v.x << ", Y: " << v.y << ", Z: " << v.z;
    // We return the stream so we can chain it!! (e.g. cout << v1 << v2;)
    return stream;
}


int main(){
    std::cout << "========== TOPIC: Operators and Operator Overloading ==========" << std::endl;

    std::cout << "EXAMPLE: The Core Syntax understanding with \'+\' & \'*\' operator overloading" << std::endl;
    Vector3 position(10.0f, 10.0f, 5.0f);
    Vector3 velocity(1.0f, 2.0f, 0.0f);
    std::cout << "Position Vector Values (x,y,z): " << position.x << "," << position.y << "," << position.z << std::endl;
    std::cout << "Velocity Vector Values (x,y,z): " << velocity.x << "," << velocity.y << "," << velocity.z << std::endl;

    // Beautiful, readale C++ code!
    Vector3 next_position = position + velocity;
    std::cout << "New Position Vector Values (x,y,z): " << next_position.x << "," << next_position.y << "," << next_position.z << std::endl;

    // Scaling the vector
    Vector3 fast_velocity = velocity * 2.0f;
    std::cout << "New Velocity Vector Values (x,y,z): " << fast_velocity.x << "," << fast_velocity.y << "," << fast_velocity.z << std::endl;

    std::cout << "EXAMPLE: The Equality Operator \'==\' operator overloading" << std::endl;
    SensorConfig current_config = {115200, 4, 5};
    SensorConfig gps_config = {9600, 4, 5};
    SensorConfig rain_sensor_config = {115200, 4, 5};

    if(current_config == rain_sensor_config){
        std::cout << "SUCCESS: current config == rain sensor config" << std::endl;
    }
    else{
        std::cout << "FAILED: current config != rain sensor config" << std::endl;
    }

    if(current_config != gps_config){
        std::cout << "SUCCESS: current config != gps config" << std::endl;
    }
    else{
        std::cout << "FAILED: current config == gps config" << std::endl;
    }

    std::cout << "EXAMPLE: The Left Shift \'<<\' operator overloading with cout" << std::endl;
    Vector3 drone_pos(14.5f, 9.2f, 100.0f);
    // std::cout now knows exactly how to print a Vector3!
    std::cout << "Drone Location: " << drone_pos << std::endl;

    std::cout << "===============================================================" << std::endl;
    std::cin.get();
}