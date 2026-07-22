// Topic: Dynamic Arrays in C++
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <memory>

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

// Mock Interface
class ISensor{
public:
    virtual void read() = 0;
    virtual ~ISensor() = default;
};

class TempSensor : public ISensor {
public:
    TempSensor(){
        std::cout << "[CONSTRUCTOR] TEMP SENSOR OBJECT INSTANTIATED!\n";
    }

    ~TempSensor(){
        std::cout << "[DESTRUCTOR] TEMP SENSOR OBJECT DESTROYED!\n";
    }

    void read() override {
        std::cout << "Reading Temp: 24*C" << std::endl;
    }
};

class PressureSensor : public ISensor {
public:
    PressureSensor(){
        std::cout << "[CONSTRUCTOR] PRESSURE SENSOR OBJECT INSTANTIATED!\n";
    }

    ~PressureSensor(){
        std::cout << "[DESTRUCTOR] PRESSURE SENSOR OBJECT DESTROYED!\n";
    }

    void read() override {
        std::cout << "Reading Pressure: 955.38 hpa" << std::endl;
    }
};

// Optimizig the usage of std::vector in C++
struct Vertex{
    float x,y,z;

    Vertex(float x, float y, float z) : x(x), y(y), z(z) {};

    Vertex(const Vertex& vertex) : x(vertex.x), y(vertex.y), z(vertex.z) {
        std::cout << "Copied Occured!" << std::endl;
    }
};

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

    // EXAMPLE: Clean Iteration (Using Auto)
    std::vector<std::string> active_tasks = {"UART_Read", "SPI_Write", "I2C_Poll"};

    // 1. The old, clunky C-stle way:
    for (size_t i = 0; i < active_tasks.size(); i++){
        std::cout << active_tasks[i] << std::endl;
    }

    // 2. The Modern C++ Way (Range-based for loop):
    // DANGER: Using plain 'auto_task' creates a slow copy of each string!
    // FIX: We use 'const auto&` to pass-by-reference safely.
    for(const auto& task : active_tasks){
        std::cout << task << std::endl;
    }

    // EXAMPLE: Combining Vectors and Smart Pointers
    // A Dynamic array that exclusively owns Polymorphic Hardware Drivers!
    std::vector<std::unique_ptr<ISensor>> system_sensors;

    // We use push_back combined with make_unique
    system_sensors.push_back(std::make_unique<TempSensor>());
    system_sensors.push_back(std::make_unique<PressureSensor>());

    // Iterate and Execute Dynamic Dispatch
    for (const auto& sensor : system_sensors){
        sensor->read();
    }

    // When main() ends, the vector is destroyed.
    // The vector automatically destroys all the unique_ptrs.
    // The unique_ptrs automatically delete the Temp and Pressure objects!
    // 100% leak-proof architecture.

    // EXAMPLE: Removing Elements
    std::vector<int> numbers = {10,20,30,40};
    std::cout << "Size of the numbers vector: " << numbers.size() << std::endl;
    std::cout << "Capacity of the numbers vector: " << numbers.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < numbers.size() ; i++){
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Erase the 2nd element i.e. 20
    // 30 & 40 mustbe left shifted in memory to maintain contiguous
    numbers.erase(numbers.begin() + 1);     // Size: 3, Capacity: 4
    std::cout << "Size of the numbers vector: " << numbers.size() << std::endl;
    std::cout << "Capacity of the numbers vector: " << numbers.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < numbers.size() ; i++){
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // Clear the entire vector
    numbers.clear(); // Size: 0, Capacity: usually remains the same i.e. 4
    std::cout << "Size of the numbers vector: " << numbers.size() << std::endl;
    std::cout << "Capacity of the numbers vector: " << numbers.capacity() << std::endl;
    std::cout << "All Elements in the Vector: ";
    for(int i=0; i < numbers.size() ; i++){
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    // EXAMPLE: Optimizig the usage of std::vector in C++
    std::vector<Vertex> vertices;
    vertices.push_back(Vertex(1,2,3));
    vertices.push_back(Vertex(4,5,6));
    vertices.push_back(Vertex(7,8,9));
    /* Note
     * Vertex object is constructed in main stack and then later copied to the memory of the vertices
     * At first push, V1 Copied to vector index 0, and vector capacity is 1, no reallocation. therefore only 1 copy occurred.
     * At second push, V2 copied to vector index 1, and vector resizes to 2. V1 is copied again to new memory, therefore 2 copes occurred
     * At third push, V3 copied to vector index 2, vector resizes to 3. V1 & V2 are copied again to new memory, therefore 3 copies occurred
     * Therefore in total 6 copies occurred, which hits performance hard!
    */
    std::cout << "Optimization implemented below, therefore no copies will be printed below\n" << std::endl;
    std::vector<Vertex> optimized_vertices;
    optimized_vertices.reserve(3);  // Makes sure that we have enough memory, therefore removing reallocations
    // optimized_vertices.push_back(Vertex(1,2,3));
    // optimized_vertices.push_back(Vertex(4,5,6));
    // optimized_vertices.push_back(Vertex(7,8,9));
    // Therefore now only three copies

    optimized_vertices.emplace_back(1,2,3);
    optimized_vertices.emplace_back(4,5,6);
    optimized_vertices.emplace_back(7,8,9);
    // This method, also removes the three copies, by just passing the parameter list for the constructor, instead ob constructed objects.

    std::cout << "==================================================\n";
    std::cin.get();
}