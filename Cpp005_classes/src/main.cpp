/* CLASSES IN C++
*/

#include "iostream"

class log{
public:
    static constexpr int LOG_LEVEL_ERROR = 0;
    static constexpr int LOG_LEVEL_WARNING = 1;
    static constexpr int LOG_LEVEL_INFO = 2;
private:
    int _log_level;

public:
    // Default Constructor
    log(){
        _log_level = LOG_LEVEL_INFO;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << _log_level << std::endl;
    }
    
    // Default Destructor
    ~log(){
        std::cout << "LOG INSTANCE WILL BE DESTROYED DUE TO DESTRUCTOR BEING TRIGGERED" << std::endl;
    }
    
    // Constructor with Log Level input
    log(int level){
        _log_level = level;
        std::cout << "LOG INSTANCE CREATION SUCCESS - WITH LOG LEVEL: " << _log_level << std::endl;
    }

    void set_log_level(int level){
        _log_level = level;
    }

    void error(const char* message){
        if(_log_level >= LOG_LEVEL_ERROR)
            std::cout << "[ERROR]" << " " << message << std::endl;
    }

    void warning(const char* message){
        if(_log_level >= LOG_LEVEL_WARNING)
        std::cout << "[WARNING]" << " " << message << std::endl;
    }

    void info(const char* message){
        if(_log_level >= LOG_LEVEL_INFO)
        std::cout << "[INFO]" << " " << message << std::endl;
    }
};


int main(){
    log logger(log::LOG_LEVEL_INFO);
    // logger.set_log_level(logger.LOG_LEVEL_INFO);
    logger.info("LOGGER INTIALIZED");
    logger.warning("RESPONSE CRC FAILED, DROPPING MSG");
    logger.error("HARDWARE FAILED TO INITALIZE");

    std::cin.get();
}