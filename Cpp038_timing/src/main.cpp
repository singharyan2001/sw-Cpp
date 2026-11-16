#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <cstdint>

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

class Trial{
public:
    Trial(){
        //
    }

    ~Trial(){
        //
    }
private:
    std::chrono::steady_clock::time_point m_start, m_end;
    std::chrono::duration<long long, std::nano> m_interval_ns;  // Nanoseconds
    std::chrono::duration<long long, std::milli> m_interval_ms; // Milliseconds
    std::chrono::duration<int, std::ratio<60>> m_interval_s;    // Seconds
};

int main(){
    std::cout << "========== Timings in CPP ==========\n";
    
    #if TIMER_METHOD == 1
    auto start =  std::chrono::high_resolution_clock::now();
    #endif

    // std::thread worker(DoWork);

    // std::cin.get();
    // s_finished = true;

    // worker.join();

    // std::cout << "Finished...\n";

    #if TIMER_METHOD == 1
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;
    std::cout << "[TIMING] Duration: " << duration.count() << std::endl;
    #endif

    /* New Examples */
    std::chrono::steady_clock::time_point start1, end1;
    std::chrono::duration<long long, std::nano> interval_ns;  // Nanoseconds'
    // note: Could have also used - std::chrono::miliseconds/nanoseconds/microseconds and so on.
    std::chrono::duration<long long, std::micro> interval_us; // Microseconds
    std::chrono::duration<long long, std::milli> interval_ms; // Milliseconds
    std::chrono::duration<double, std::ratio<1>> interval_s;    // Seconds

    std::chrono::microseconds interval2_us;

    // Timepoint Example
    start1 = std::chrono::steady_clock::now(); // Start the timer
    // Do some work
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    end1 = std::chrono::steady_clock::now();    // Stop the timer
    // Calculate duration
    // When you subtract two timepoints, you get a duration, but you need to typecaste
    // the duration to respective unit of time like milliseconds and so on
    auto elapsed = end1 - start1;
    interval_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);
    interval_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
    interval2_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
    interval_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    interval_s = elapsed; // Note: No duration cast needed when converting to float seconds
    std::cout << "Time Elapsed in Tns: " << interval_ns.count() << " ns\n";
    std::cout << "Time Elapsed in Tus: " << interval2_us.count() << " us\n";
    std::cout << "Time Elapsed in Tms: " << interval_ms.count() << " ms\n";
    std::cout << "Time Elapsed in Ts: " << interval_s.count() << " s\n";

    // Timestamp Generation
    auto gen_timestamp = std::chrono::system_clock::now();
    std::cout << "Timestamp Generated (ms): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(gen_timestamp.time_since_epoch()).count() 
              << std::endl;

    // Format as YYYY-MM-DD HH:MM:SS in local time
    std::time_t raw_time = std::chrono::system_clock::to_time_t(gen_timestamp);
    std::cout << "Timestamp Generated: "
    << std::put_time(std::localtime(&raw_time), "%Y-%m-%d %H:%M:%S")
              << std::endl;
    //

    // Timestamp generation and storing in int64_t
    // 1. Capture the current time
    auto current_now = std::chrono::system_clock::now();
    // 2. Convert to your desired unit and store in int64_t
    int64_t timestamp_s = std::chrono::duration_cast<std::chrono::seconds>(
        current_now.time_since_epoch()).count();

    int64_t timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        current_now.time_since_epoch()).count();

    int64_t timestamp_us = std::chrono::duration_cast<std::chrono::microseconds>(
        current_now.time_since_epoch()).count();

    // 3. Print them out
    std::cout << "Seconds:      " << timestamp_s  << "\n";
    std::cout << "Milliseconds: " << timestamp_ms << "\n";
    std::cout << "Microseconds: " << timestamp_us << "\n";
    
    std::cin.get();
}
