#include <iostream>
#include <thread>

static bool s_finished = false;

void DoWork(){
    using namespace std::literals::chrono_literals;
    std::cout << "Started thread id=" << std::this_thread::get_id() << std::endl;
    while(!s_finished){
        std::cout << "Working...\n";
        std::this_thread::sleep_for(1s);
    }
}

int main(){
    std::cout << "========== Threads in CPP ==========\n";
    
    std::thread worker(DoWork);

    std::cin.get();
    s_finished = true;

    worker.join();

    std::cout << "Finished...\n";

    std::cin.get();
}
