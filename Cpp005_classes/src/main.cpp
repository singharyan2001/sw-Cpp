/* CLASSES IN C++
*/

#include "iostream"

class log{
    public:
        const int LOG_LEVEL_ERROR = 0;
        const int LOG_LEVEL_WARNING = 1;
        const int LOG_LEVEL_INFO = 2;
    private:
        int _log_level = LOG_LEVEL_INFO;
    
    public:
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
    log logger;
    logger.set_log_level(logger.LOG_LEVEL_INFO);
    logger.info("LOGGER INTIALIZED");
    logger.warning("RESPONSE CRC FAILED, DROPPING MSG");
    logger.error("HARDWARE FAILED TO INITALIZE");
}