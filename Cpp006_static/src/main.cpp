/* Topics
 * Static in Local functions, static in global space and static in classes & Structs
*/

#include<iostream>

#define SYSCLK              16000000
#define FPCLK               SYSCLK
#define OVERSAMPLING        16

inline void log_msg(const char* message){
    std::cout << message << std::endl;
}

inline void log_data(const char* message, int& data){
    std::cout << message <<  data << std::endl;
}

/* Note
 * This API utilizes a local static variable which retains its value, even after
*/
void print_call(){
    static int x = 0;
    log_data("VALUE OF STATIC X: ", x);
    x++;
}

/* Note
 * This API cannot be called by any other file
*/
static int calculate_baudrate(int& baud){
    return (FPCLK)/(OVERSAMPLING*baud);
}

// Structure implementation example
struct GPS{
    static double longitude, latitude;

    static void print_coordinates(){
        std::cout << "Latitude: " << latitude << std::endl;
        std::cout << "Longitude: " << longitude << std::endl;
    }
};

// Declaration
double GPS::latitude;
double GPS::longitude;

class gps{
public:
    static double longitude, latitude;

    static void print_coordinates(){
        std::cout << "Latitude: " << latitude << std::endl;
        std::cout << "Longitude: " << longitude << std::endl;
    }
};

// Declaration
double gps::latitude;
double gps::longitude;

int main(){
    log_msg("======= TOPIC: Static Keyword in C++ ============");

    log_msg("============= local Static in C++ ===============");
    for(int i=0; i<20; i++){
        print_call();
    }
    log_msg("=================================================");

    log_msg("========== Static used in Global Space ==========");
    int desired_baud_rate = 9600;
    int baud_register_value = calculate_baudrate(desired_baud_rate);
    log_data("Calculated Baud Rate Register Value: ", baud_register_value);
    log_msg("=================================================");

    log_msg("============= Static in Structs =================");
    GPS::latitude = 73.3847303;
    GPS::longitude = 19.3862920;
    GPS::print_coordinates();
    log_msg("=================================================");

    log_msg("============= Static in Class =================");
    gps::latitude = 73.23433;
    gps::longitude = 17.45323;
    gps::print_coordinates();
    log_msg("=================================================");
 
    std::cin.get();
}
