#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <string>
#include <vector>
#include <queue>

// ---- Shared Resources ----
std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> tx_queue;
bool is_running = true;

// 1. The Consumer (Tx thread)
void txThread(){
    std::cout << "[TX Thread] Started. Waiting for cmds...\n";
    while(is_running){
        // Step 1: Acquire the Unique Lock
        std::unique_lock<std::mutex> lock(mtx);

        // Step 2: Go to sleep until the queue has data OR we are shutting down.
        // The lock is AUTOMATICALLY UNLOCKED while sleeping here!
        cv.wait(lock, []{return !tx_queue.empty() || !is_running; });

        // Step 3: Woke Up! (The lock is automatically RE-LOCKED here)
        // If shutting down and the queue is empty, exit the loop
        if(!is_running && tx_queue.empty()){
            break;
        }

        // Step 4: Safely grab the command
        std::string cmd = tx_queue.front();
        tx_queue.pop();

        // Step 5: UNLOCK EARLY
        // We have the data, we don't need to lock the queue while we talk to hardware
        lock.unlock();

        // Step 6: Dispatch to RS485
        std::cout << "[TX THREAD] Dispatching to hardware: " << cmd << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulating baud rate delay
    }
    std::cout << "[TX Thread] Exiting cleanly.\n";
}

// 2. The Producer (App layer)
void appThread(){
    std::cout << "[APP Thread] Booting up UAV Application...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Send 3 commands with pauses in between
    for(int i = 1; i <= 3; i++){
        std::string cmd = "CMD_UAV_FLAP_OPEN (Seq" + std::to_string(i) + ")";

        { // Use a strict scope for the push
            std::lock_guard<std::mutex> lock(mtx);
            tx_queue.push(cmd);
            std::cout << "\n[App Thread] Pushed to queue: " << cmd << "\n";
        } // Lock is released here

        // RING THE BELL! Wake up the TX Thread.
        cv.notify_one();
        // Simulate app doing other things
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    // Initiate Graceful Shutdown
    std::cout << "\n[App Thread] Initiating system shutdown...\n";
    {
        std::lock_guard<std::mutex> lock(mtx);
        is_running = false;
    }
    // Ring the bell one last time so the TX thread wakes up to see is_running = false
    cv.notify_all();
}

int main(){
    // Spawn threads
    std::thread tx(txThread);
    std::thread app(appThread);

    // Wait for completion
    app.join();
    tx.join();

    return 0;
}

/* MY OUTPUT
[TX Thread] Started. Waiting for cmds...
[APP Thread] Booting up UAV Application...

[App Thread] Pushed to queue: CMD_UAV_FLAP_OPEN (Seq1)
[TX THREAD] Dispatching to hardware: CMD_UAV_FLAP_OPEN (Seq1)

[App Thread] Pushed to queue: CMD_UAV_FLAP_OPEN (Seq2)
[TX THREAD] Dispatching to hardware: CMD_UAV_FLAP_OPEN (Seq2)

[App Thread] Pushed to queue: CMD_UAV_FLAP_OPEN (Seq3)
[TX THREAD] Dispatching to hardware: CMD_UAV_FLAP_OPEN (Seq3)

[App Thread] Initiating system shutdown...
[TX Thread] Exiting cleanly.
*/

/* The Key Takeaways from the Output:
Execution workflow of the above example
1. txThread starts, locks the mutex, and hits cv.wait(). Seeing the queue is empty, it unlocks the mutex and goes to sleep. CPU usage drops to 0%.
2. appThread generates a command, locks the mutex using lock_guard, pushes to the queue, and unlocks.
3. appThread calls cv.notify_one().
4. The OS instantly wakes up txThreadFunc.
5. txThread re-locks the mutex, checks the lambda (!tx_queue.empty()), sees it is true, and proceeds to pop the queue.
6. txThread unlocks manually, spends 50ms talking to the hardware, then loops back to wait for the next bell.
*/