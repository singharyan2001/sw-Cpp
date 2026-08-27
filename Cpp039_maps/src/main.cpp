#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <unordered_map>

/*=================================== MAPS EXAMPLE ====================================*/
struct CityRecord
{
    std::string Name;
    uint64_t Population;
    double latitude, longitude;
};

namespace std{
    template<>
    struct hash<CityRecord>
    {
        size_t operator()(const CityRecord& key)
        {
            return hash<std::string>()(key.Name);
        }
    };
}

void vector_example(){
    // Create Vector
    std::vector<CityRecord> cities;

    // Populate vector
    cities.push_back({"Melboune", 50000000, 2.4, 9.4});
    cities.push_back({"Sydney", 90000000, 3.4, 7.4});
    cities.push_back({"Berlin", 70000000, 4.4, 5.4});
    cities.push_back({"London", 100000000, 5.4, 3.4});
    cities.push_back({"Paris", 90000000, 1.4, 1.4});

    // Perform Operation on the Vector
    for(const auto& city : cities){
        if(city.Name == "Berlin"){
            std::cout << "BERLIN FOUND!!\n";
            break;
        }
        else{
            std::cout << "NOT FOUND!!\n";
        }
    }
}

void map_example()
{
    // Create MAP (Ordered)
    std::map<std::string, CityRecord> cityMap;

    // Populate MAP
    cityMap["Melboune"] = CityRecord {"Melboune", 50000000, 2.4, 9.4};
    cityMap["Sydney"] = CityRecord {"Sydney", 90000000, 3.4, 7.4};
    cityMap["Berlin"] = CityRecord {"Berlin", 70000000, 4.4, 5.4};
    cityMap["London"] = CityRecord {"London", 100000000, 5.4, 3.4};
    cityMap["Paris"] = CityRecord {"Paris", 90000000, 1.4, 1.4};

    // ACCESS MAP
    CityRecord& BerlinData = cityMap["Berlin"];
    std::cout << "Population: " << BerlinData.Population << std::endl;
}

void unmap_example()
{
    // Create MAP (Ordered)
    std::unordered_map<std::string, CityRecord> cityMap;

    // Populate MAP
    cityMap["Pune"] = CityRecord {"Pune", 10000000, 2.4, 9.4};
    cityMap["Nashik"] = CityRecord {"Nashik", 5000000, 3.4, 7.4};
    cityMap["Mumbai"] = CityRecord {"Mumbai", 30000000, 4.4, 5.4};
    cityMap["Nagpur"] = CityRecord {"Nagpur", 10000000, 5.4, 3.4};
    cityMap["Satara"] = CityRecord {"Satara", 800000, 1.4, 1.4};

    // ACCESS MAP
    CityRecord& MumbaiData = cityMap["Mumbai"];
    std::cout << "Population: " << MumbaiData.Population << std::endl;
}

// void custom_hashed_unmap_example(){
//     std::unordered_map<CityRecord, uint32_t> foundedMap;
//     foundedMap[CityRecord {"Pune", 10000000, 2.4, 9.4}] = 1850;
//     foundedMap[CityRecord {"Nashik", 5000000, 3.4, 7.4}] = 1847;
//     foundedMap[CityRecord {"Mumbai", 30000000, 4.4, 5.4}] = 1778;
// }

/*=================================== TIMER AND THREAD CODE START ====================================*/
// METHOD 1 -> 1, METHOD 2 -> 2
#define TIMER_METHOD    2

static bool s_finished = false;

class Timer{
private:
    std::chrono::high_resolution_clock::time_point m_start, m_end;
    std::chrono::duration<float> m_duration;
public:
    Timer(){
        m_start = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        m_end = std::chrono::high_resolution_clock::now();
        m_duration = m_end - m_start;
        std::cout << "[TIMING] Duration: " << m_duration.count() << std::endl;
    }
};

void DoWork(){
    #if TIMER_METHOD == 2
    Timer timer;
    #endif

    using namespace std::literals::chrono_literals;
    std::cout << "Started thread id=" << std::this_thread::get_id() << std::endl;

    // std::this_thread::sleep_for(1s);
    vector_example();
    map_example();
    unmap_example();
    // std::this_thread::sleep_for(1s);
}

/*=================================== TIMER AND THREAD CODE ENDS ====================================*/

int main(){
    std::cout << "========== Timings in CPP ==========\n";
    
    #if TIMER_METHOD == 1
    auto start =  std::chrono::high_resolution_clock::now();
    #endif

    std::thread worker(DoWork);

    std::cin.get();
    s_finished = true;

    worker.join();

    std::cout << "Finished...\n";

    #if TIMER_METHOD == 1
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    std::cout << "[TIMING] Duration: " << duration.count() << std::endl;
    #endif

    std::cin.get();
}
