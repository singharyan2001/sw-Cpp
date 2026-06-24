#include<iostream>

inline void log_msg(const char* message){
    std::cout << message << std::endl;
}

inline void log_data(const char* message, int& data){
    std::cout << message <<  data << std::endl;
}

int main(){
    log_msg("TOPIC: Static Keywork in C++");
}
