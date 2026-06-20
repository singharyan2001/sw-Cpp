#ifndef LOG_H
#define LOG_H

#include <iostream>

inline void log_msg(const char* msg){
    std::cout << msg << std::endl;
}

inline void log_data(const char* msg, int data){
    std::cout << msg << data << std::endl;
}

// Operator Overloading example
inline void log_data(const char* msg, void* ptr){
    std::cout << msg << ptr << std::endl;
}

#endif // LOG_H