#include <iostream>
#include <chrono>
#include <thread>

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
    while(!s_finished){
        std::cout << "Working...\n";
        std::this_thread::sleep_for(1s);
    }
}

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
