// Topic: Dynamic Arrays in C++
#include <iostream>
#include <vector>
#include <cstdint>

// EXAMPLE: The Golden Optimization: `.reserve()`
void processNetworkPayload(int expected_bytes){
    std::vector<uint8_t> rx_buffer;

    // OPTIMIZATION: We ask the OS for memory exactly once!
    rx_buffer.reserve(expected_bytes);

    uint8_t count = 7;
    
    for(int i=0; i < expected_bytes ; i++){
        // becasue of .reserve(), this is now as fast as writing to a raw C array!
        rx_buffer.push_back(count++);
    }

    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < expected_bytes ; i++){
        std::cout <<static_cast<int>(rx_buffer[i]) << " ";
    }
    std::cout << std::endl;
}

int main(){
    std::cout << "========== TOPIC: Dynamic Arrays in C++ ==========\n";
    
    // Example: The Reallocation Penalty
    std::vector<int> sensor_data;
    // Size: 0, Capacity: 0
    
    sensor_data.push_back(10);  // vector allocates memory for 1 item
    // Size: 1, Capacity: 1
    std::cout << "Size of the sensor_data vector: " << sensor_data.size() << std::endl;
    std::cout << "Capacity of the sensor_data vector: " << sensor_data.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < sensor_data.size() ; i++){
        std::cout << sensor_data[i] << " ";
    }
    std::cout << std::endl;

    sensor_data.push_back(20);  // vector is full, It allocates space for 2 items, copies the '10', and deletes the old memory.
    // Size: 2, Capacity: 2
    std::cout << "Size of the sensor_data vector: " << sensor_data.size() << std::endl;
    std::cout << "Capacity of the sensor_data vector: " << sensor_data.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < sensor_data.size() ; i++){
        std::cout << sensor_data[i] << " ";
    }
    std::cout << std::endl;

    sensor_data.push_back(30);  // Full again! allocates space for 4 items, copies 10 & 20...
    // Size: 3, Capacity: 4
    std::cout << "Size of the sensor_data vector: " << sensor_data.size() << std::endl;
    std::cout << "Capacity of the sensor_data vector: " << sensor_data.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < sensor_data.size() ; i++){
        std::cout << sensor_data[i] << " ";
    }
    std::cout << std::endl;

    // EXAMPLE: The Golden Optimization: `.reserve()`
    processNetworkPayload(15);

    std::cout << "==================================================\n";
    std::cin.get();
}