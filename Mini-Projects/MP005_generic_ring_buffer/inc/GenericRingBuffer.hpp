#ifndef GENERIC_RING_BUFFER_HPP
#define GENERIC_RING_BUFFER_HPP

#include <memory>

template <typename T>
class RingBuffer{
private:
    std::unique_ptr<T []> m_data;
    size_t m_capacity, m_head, m_tail, m_bytes_written;
public:
    // Note: RULE OF FIVE & RULE OF ZERO Design principles followed
    // CONSTRUCTOR
    explicit RingBuffer(size_t capacity) 
      : m_capacity(capacity),
        m_data(std::make_unique<T []>(capacity)),
        m_head(0),
        m_tail(0),
        m_bytes_written(0)
    {
        std::cout << "[CONSTRUCTOR] RING BUFFER OBJECT INSTANTIATED\n";
    }

    // DESTRUCTOR
    ~RingBuffer() = default;

    // COPY CONSTRUCTOR
    // Deleting the Copy Constructor & Copy Assignment Operator of the class
    RingBuffer(const RingBuffer& other) = delete;               // Copy Constructor deleted
    RingBuffer& operator=(const RingBuffer& other) = delete;    // Copy Operator deleted

    // MOVE CONSTRUCTOR
    // Move Constructor
    RingBuffer(RingBuffer&& other) noexcept
      : m_capacity(other.m_capacity), 
        m_head(other.m_head), m_tail(other.m_tail), 
        m_bytes_written(other.m_bytes_written), 
        m_data(std::move(other.m_data))
    {
        // CRITICAL STEP: Reset the source object
        other.m_capacity = 0;
        other.m_bytes_written = 0;
        other.m_head = 0;
        other.m_tail = 0;
        std::cout << "[MOVE CONSTRUCTOR] RING BUFFER MOVED TO NEW RESOURCE\n";
    }

    // Move Assignment Operator
    RingBuffer& operator=(RingBuffer&& other) noexcept {
        std::cout << "[MOVE ASSIGMENT] TRANSFERRING OWNERSHIP TO NEW OBJECT\n";

        // Guard against self assignment
        if(this != &other){
            // Stealing Resources
            m_data = std::move(other.m_data);
            m_capacity = other.m_capacity;
            m_bytes_written = other.m_bytes_written;
            m_head = other.m_head;
            m_tail = other.m_tail;

            // CRITICAL STEP: RESET SOURCE
            other.m_capacity = 0;
            other.m_bytes_written = 0;
            other.m_head = 0;
            other.m_tail = 0;
        }
        return *this;
    }

    //Operator Overloading [] for writing and reading buffer
    T& operator[](size_t index) noexcept {
        return m_data[index];
    }

    // GENERIC RING BUFFER FUNCTIONALTY
    
    bool push(T value){
        // Check for Full condition
        if(isFull()){
            return false;
        }
        // Perform Push Operation
        m_data[m_head] = value;
        // Safe Warp Around based
        m_head = (m_head + 1) % m_capacity;
        m_bytes_written++;
        return true;
    }

    bool pop(T& out_value){
        // Check for Empty Condition
        if(isEmpty()){
            return false;
        }
        // Perform POP operation
        out_value = m_data[m_tail];
        m_tail = (m_tail + 1) % m_capacity;
        m_bytes_written--;
        return true;
    }

    bool isEmpty(){
        // Check if buffer is empty
        if( (m_head == m_tail) && (m_bytes_written == 0) ){
            return true;
        }
        else{
            return false;
        }
    }

    bool isFull(){
        // Check for Full condition
        if(m_bytes_written == m_capacity){
            return true;
        }
        else{
            return false;
        }
    }

    void print_status(){
        // Print out the generic ring buffer status
        std::cout << "==== RING BUFFER INFO [START] ====\n";
        std::cout << "CURRENT RING BUFFER ELEMENTS: ";
        for(int i=0; i < m_bytes_written; i++){
            // Calculate the true memory index
            size_t actualIndex = (m_tail + i) % m_capacity;
            // Print the Element
            std::cout << m_data[actualIndex] << ", ";
        }
        if(m_bytes_written == 0){
            std::cout << "NULL";
        }
        std::cout << std::endl;
        std::cout << "RING BUFFER CAPACITY: " << m_capacity << std::endl;
        std::cout << "RING BUFFER HEAD INDEX: " << m_head << std::endl;
        std::cout << "RING BUFFER TAIL INDEX: " << m_tail << std::endl;
        std::cout << "RING BUFFER isEmpty: " << isEmpty() << std::endl;
        std::cout << "RING BUFFER isFull: " << isFull() << std::endl;
        std::cout << "==== [STOP] ====\n";
    }
};

#endif // GENERIC_RING_BUFFER_HPP