// Templates in C++
#include <iostream>

/*================================= FUNCTION TEMPLATE ===============================*/
// The tedious, procedural C way
int max_int(int a, int b){ 
    return (a > b) ? a : b;
}

float max_float(float a, float b){ 
    return (a > b) ? a : b;
}

// C++ WAY
// 1. Declare the template and define a placeholder typename 'T'
template <typename T>
T get_max(T a, T b){
    return (a > b) ? a : b;
}

/*============================ CLASS TEMPLATES EXAMPLE ==============================*/
// The Blueprint for a generic buffer
template <typename T>
class Buffer{
private:
    T* data;
    int size;
public:
    Buffer(int s) : size(s) {
        data = new T[size]; // Allocates memory for whatever 'T' is.
        std::cout << "[BUFFER CONSTRUCTOR] BUFFER INSTANTIATED!\n";
    }

    ~Buffer(){
        delete[] data;
        std::cout << "[BUFFER DESTRUCTOR] BUFFER DESTROYED!\n";
    }

    void write(int index, T value){
        if(index < size){
            std::cout << "[BUFFER] Write Valid! Writing data to buffer!\n";
            data[index] = value;
        }
    }

    T read(int index) const {
        std::cout << "[BUFFER] Read Valid!, Reading data from buffer!\n";
        return data[index];
    }
};

/*============================== TEMPLATE PARAMETERS ===================================*/
// T is the data type, N is the exact size of the array
template <typename T, size_t N>
class StaticArray {
private:
    T m_data[N];    // The Compiler knows 'N', so this sits cleanly on the stack.
public:
    int get_size() const {
        return N;
    }
};

int main(){
    std::cout << "============ TOPIC: TEMPLATES IN C++ ============" << std::endl;
    // 2. The Compiler sees this and physically generated get_max(int, int)
    int highest_int = get_max<int>(5, 10);
    std::cout << "Highest Integer: " << highest_int << std::endl;

    // 3. The Compiler generates get_max(float, float)
    float highest_float = get_max<float>(7.7f, 10.10f);
    std::cout << "Highest Float: " << highest_float << std::endl;

    // 4. Implicit Deduction: You can usually drop the <type> brackets entirely!
    double highest_double = get_max(99.9, 100.1);
    std::cout << "Highest Double: " << highest_double << std::endl;

    // Instantiating a Buffer designed specifically for floats
    Buffer<float> temp_sensor_log(10);
    temp_sensor_log.write(0, 2.45f);
    std::cout << "Validating Buffer Data at index 0: \n" << temp_sensor_log.read(0) << std::endl; 

    // Instantiating a Buffer designed specifically for chars
    Buffer<char> uart_rx_buffer(256);
    uart_rx_buffer.write(0, 'A');
    std::cout << "Validating Buffer Data at Index 0: \n" << uart_rx_buffer.read(0) << std::endl;

    // Generate a class with an internal float array of size 100
    StaticArray<float, 100> temp_readings;
    float length = temp_readings.get_size();
    std::cout << "Temperature Reading Static Array Size: " << length << std::endl;

    // Generate a completely different class with an int array of size 5
    StaticArray<int, 5> status_flags;
    int flag_array_length = status_flags.get_size();
    std::cout << "Status Flag Static Array Size: " << flag_array_length << std::endl;

    std::cout << "=================================================" << std::endl;
    std::cin.get();
}