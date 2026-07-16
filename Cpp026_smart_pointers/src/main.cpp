// Smart Pointers in C++
#include <iostream>
#include <memory>

#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <algorithm>

//
class Sensor{
private:
    int m_address;
    inline static int m_count = 0;
public:
    Sensor(int addr) : m_address(addr) {
        std::cout << "Sensor Object Instantiated \n";
        m_count++;
        printCount();
    }

    ~Sensor(){
        std::cout << "Sensor Object Destroyed \n";
        m_count--;
        printCount();
    }

    void printTemp(float data){
        std::cout << "[SENSOR DATA] Temp: " << data << std::endl;
    }
    
    void printCount(){
        std::cout << "Current Count: " << m_count << std::endl;
    }
};

//========================= Unique Pointer Concept ========================================
class IDriver {
public:
    virtual void configure() = 0;
    virtual ~IDriver() {
        std::cout << "[IDiver] Destructor exectued cleanly.\n";
    }
};

class UartDriver : public IDriver {
public:
    UartDriver() {
        std::cout << "[Uart] hardware configured on UART1 \n";
    }

    ~UartDriver() override {
        std::cout << "[Uart] Hardware port closed \n";
    }

    void configure() override {
        std::cout << " -> Adjusting baud rate to 115200...\n";
    }
};

void runCommRoutine(){
    std::cout << "--- Entering runCommRoutine() ---\n";

    // Standard Way to instantiate: std::make_unique<T>()
    // This is safer and more exception-safe than: std::unique_ptr<UartDriver>(new UartDriver())
    std::unique_ptr<IDriver> commPort = std::make_unique<UartDriver>();
    commPort->configure();

    // COMPILER ERROR! Copying is strictly prohibited:
    // std::unique_ptr<IDriver> copiedPort = comPort;

    // Move SemanticsL Transferring ownership to another scoped block
    std::unique_ptr<IDriver> movedPort = std::move(commPort);
    if(!commPort) {
        std::cout << "[Info] 'commPort' is now empty (Null). Ownership moved to 'movedPort' \n ";
    }

    std::cout << "--- Exiting runCommRoutine() ---\n";
    // Automatic cleanup: 'movedPort' falls out of scope here.
    // The heap-allocated UartDriver is automaticallt deleted!
}

//==============================================================================================

//============================== Shared Pointer Concept ========================================
class SharedLogBuffer {
public:
    SharedLogBuffer(){
        std::cout << "[ALLOC] GLobal DMA Log Buffer Allocated \n";
    }

    ~SharedLogBuffer(){
        std::cout << "[FREE] DMA Log Buffer dellocated \n";
    }

    void writeLog(const char* text){
        std::cout << " [Buffer LOG]: " << text << std::endl;
    }
};

void runWorkerThread(std::shared_ptr<SharedLogBuffer> logger, int threadId){
    std::cout << " [Thread " << threadId << "] Working... Count: " << logger.use_count() << std::endl;
    logger->writeLog("Sensor Interrupt Processed");
    // Logger falls out of scope, count decrements
}

//==============================================================================================

//============================== Weak Pointer Concept ========================================

class Controller; // Forward Declaration

class DroneMotor {
public:
    //LOOP TRAP: if this was a std::shared_ptr, neither would ever destruct!
    std::weak_ptr<Controller> m_ControllerRef;

    ~DroneMotor(){
        std::cout << "[Motor] Despawned safely \n";
    }
};

class Controller {
public:
    std::shared_ptr<DroneMotor> m_MotorRef;

    ~Controller(){
        std::cout << "[Controller] Despawned safely \n";
    }
};

//==============================================================================================

//============================== EXAMPLE USE CASE: Unique Pointer Concept ========================================
// iostream, memory, Queue, thread, utility, vector

struct RxPacket {
    // Data Attributes
    uint16_t packetId;
    std::vector<uint8_t> payload;

    // Constructor
    RxPacket(uint16_t id, std::vector<uint8_t> data) : packetId(id), payload(data) {
        std::cout << "[ALLOC] Packet #" << packetId << " allocated in RAM.\n";
    }

    // Destructor
    ~RxPacket(){
        std::cout << "[FREE] Packet #" << packetId << " safely deallocated.\n";
    }
};

// Simulated thread-safe queue holding exclusive ownership of packets
std::queue<std::unique_ptr<RxPacket>> transitQueue;

void dmaReceiverTask(){
    std::cout << "[DMA Task] Interrupt triggered! Frame complete.\n";

    // Allocate packet exclusively
    auto newPacket = std::make_unique<RxPacket>(101, std::vector<uint8_t>{0xAA, 0xBB, 0xCC});

    // We can write to it safely here...
    newPacket->payload.push_back(0xDD);

    std::cout << "[DMA Task] Passing packet to transit queue...\n";

    // Transfer exclusive ownership to the queue.
    //After this line, 'newPacket' is nullptr and this thread can no longer access it
    transitQueue.push(std::move(newPacket));
}

/*
 * transitQueue.front():
 * 1. This looks at the item sitting at the very front of your queue (the oldest packet that has been waiting the longest).
 * 2. It returns a reference to that item, but does not remove it from the queue yet.
 * 
 * std::move(...):
 * 1. This is the critical mechanism. Because std::unique_ptr guarantees exclusive ownership, you cannot use a regular assignment (=), which would attempt to make a copy.
 * 2. std::move casts the item into an "rvalue reference", signaling to the compiler: "I am done using this item inside the queue; go ahead and strip its data out."
 * 
 * std::unique_ptr<RxPacket> activePacket = ...: This invokes the Move Constructor of your new local variable, activePacket.
 * 
 * What happens in memory during Line 1:
 * 1. activePacket takes over the memory address pointer to the RxPacket data.
 * 2. The slot at the front of transitQueue is cleared out and set to nullptr.
 * 3. The reference count or heap data is never duplicated; only the raw address pointer changes hands.
 * 
 * transitQueue.pop(): This officially deletes the element at the front of the queue and reduces the queue's size by 1.
 * 
 * Why the order matters:
 * 1. If you ran transitQueue.pop() before moving the data, the queue would instantly destroy its internal std::unique_ptr, which would trigger delete on the underlying RxPacket memory, causing a total data loss or crash.
 * 2. By running std::move first, you safely clear out the queue's pointer. When pop() is executed on the second line, it simply discards an empty nullptr shell, leaving your heap-allocated RxPacket completely intact and safely owned by your local activePacket variable.
*/
void packetParserTask(){
    if(transitQueue.empty()){
        return;
    }
    
    // POP the exclusive ownership from the queue into out local variable
    std::unique_ptr<RxPacket> activePacket = std::move(transitQueue.front());
    transitQueue.pop();

    std::cout << "[Parser Task] Safely acquired exclusive lock on Packet #" << activePacket->packetId << "\n";
    std::cout << "  -> Packet payload size: " << activePacket->payload.size() << " bytes.\n";

    // When 'activePacket' goes out of scope here, it is automatically destroyed!
    std::cout << "[Parser Task] Work complete. Leaving scope...\n";
}

//==================================================================================================

//============================== Shared Pointer Concept ========================================
// iostream, memory, thread, chrono
struct WeatherSnapshot {
    float temperature;
    float humidity;
    float pressure;
    uint64_t timestamp_ms;

    WeatherSnapshot(float t, float h, float p, uint64_t ts)
        : temperature(t), humidity(h), pressure(p), timestamp_ms(ts) {
        std::cout << "[ALLOC] Snapshot created at " << timestamp_ms << " ms\n";
    }

    ~WeatherSnapshot() {
        std::cout << "[FREE] Snapshot deleted cleanly from memory.\n";
    }
};

// Simulated tasks running on different threads/cores
void oledDisplayTask(std::shared_ptr<const WeatherSnapshot> snapshot) {
    // Local copy of pointer increments the count
    std::cout << "[OLED DISPLAY TASK] Weather Snapshot shared pointer count: " << snapshot.use_count() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Super fast
    std::cout << "  -> [OLED Task] Updated UI. Temp: " << snapshot->temperature << " C\n";
}

void sdLoggerTask(std::shared_ptr<const WeatherSnapshot> snapshot) {
    std::cout << "[LOGGER TASK] Weather Snapshot shared pointer count: " << snapshot.use_count() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Medium speed
    std::cout << "  -> [SD Task] Snapshot written to flash log.\n";
}

void wifiPublisherTask(std::shared_ptr<const WeatherSnapshot> snapshot) {
    std::cout << "[WIFI PUBLISHER TASK] Weather Snapshot shared pointer count: " << snapshot.use_count() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(80)); // Slow network delay
    std::cout << "  -> [WiFi Task] Sent telemetry payload to AWS IoT Cloud.\n";
}

//==================================================================================================

//=================================== Weak Pointer Concept =========================================
// isotram, memory, vector, algorithm
// The Interface that debugging widgets must implement
class IMUObserver{
public:
    virtual void onTelemetryUpdate(float pitch, float roll) = 0;
    virtual ~IMUObserver() = default;
};

// A Concrete Observer (OLED Display View)
class OledGraphicView : public IMUObserver{
public:
    void onTelemetryUpdate(float pitch, float roll) override {
        std::cout << "[OLED View] Drawing graph -> Pitch: " << pitch << ", Roll: " << roll << std::endl;
    }

    ~OledGraphicView(){
        std::cout << "[OLED View] Destructor: Widget closed & cleaned up.\n";
    }
};

// The central broadcaster service
class SensorBroadcaster {
private:
    std::vector<std::weak_ptr<IMUObserver>> m_Observer;
public:
    void registerObserver(std::weak_ptr<IMUObserver> observer){
        m_Observer.push_back(observer);
        std::cout << "[Broadcaster] New Observer registered \n";
    }

    void broadcast(float pitch, float roll){
        std::cout << "\n[Broadcaster] Broadcasting telemetry payload to registered observers...\n";
        // Iterate and Filter out dead observrs
        auto it = m_Observer.begin();
        while(it != m_Observer.end()){
            // Attempt to promote the weak pointer
            if(auto sharedObserver = it->lock()){
                sharedObserver->onTelemetryUpdate(pitch, roll);
                ++it;
            }
            else{
                // The Observer was destroyed in background! clean up vector.
                std::cout << "[Broadcaster] Dead observer detected. Purging from registery...\n";
                it = m_Observer.erase(it);
            }
        }
    }
};

//==================================================================================================

int main(){
    std::cout << "===================== TOPIC: SMART POINTERS IN C++ ===============\n" << std::endl;

    std::cout << "======================= TRAIL EXAMPLES/HANDS-ON ====================" << std::endl;

    // EXAMPLE: Unique Pointer in C++
    std::unique_ptr<Sensor> aht20 = std::make_unique<Sensor>(0x40);
    aht20->printTemp(24.5);

    // EXAMPLE: Shared Pointer in C++
    std::shared_ptr<Sensor> aht25 = std::make_shared<Sensor>(0x41);
    std::shared_ptr<Sensor> ptr1 = aht25;

    aht25->printTemp(30.5);
    ptr1->printTemp(27.0);

    {
        std::cout << "Entered new scope \n";
        std::unique_ptr<Sensor> aht55 = std::make_unique<Sensor>(0x44);
        std::shared_ptr<Sensor> aht30 = aht25;
        aht30->printTemp(22.4);
        aht55->printTemp(33.3);
        std::cout << "Exiting new scope \n";
    }

    // EXAMPLE: Weak Pointer in C++
    std::weak_ptr<Sensor> ptr;
    {
        std::shared_ptr<Sensor> aht30 = std::make_shared<Sensor>(0x48);
        ptr = aht30;
        aht30->printTemp(28.7);
        aht30->printCount();
    }
    if(ptr.expired()){
        std::cout << "aht25 shared pointer has expired\n";
    }
    else{
        std::cout << "aht25 shared pointer still active\n";
    }
    std::cout << "===================================================================\n" << std::endl;
    
    std::cout << "================= UNIQUE POINTER EXAMPLE USE CASE ===================" << std::endl;
    // UNIQUE POINTER USE-CASE
    runCommRoutine();

    std::cout << "================= UNIQUE POINTER EXAMPLE USE CASE II ===================" << std::endl;
    std::cout << "=== SYSTEM BOOT: DMA PIPELINE RUN ===\n\n";

    dmaReceiverTask();
    std::cout << "\n--- Queue holds ownership of packet ---\n\n";
    packetParserTask();

    std::cout << "\n=== DIAGNOSTIC RUN ENDED ===" << std::endl;
    std::cout << "=====================================================================" << std::endl;

    std::cout << "====================================================================\n" << std::endl;

    std::cout << "================= SHARED POINTER EXAMPLE USE CASE ===================" << std::endl;
    // SHARED POINTER USE-CASE
    std::cout << "--- Initializing System ---\n";

    // Rule: Always use std::make_shared
    // It is a massive performance win because it performs ONE single heap allocation containing both the managed object & the control block together.
    std::shared_ptr<SharedLogBuffer> systemLogger = std::make_shared<SharedLogBuffer>();
    std::cout << "Active Owners: " << systemLogger.use_count() << std::endl; // Outputs >> 1

    {
        std::cout << "\n--- Spawning Simulated Thread Scope ---\n";
        // COpying the shared pointer increments the reference count
        std::shared_ptr<SharedLogBuffer> threadReference = systemLogger;
        std::cout << "Active Owners: " << systemLogger.use_count() << std::endl; // Outputs >> 2

        runWorkerThread(threadReference, 1);
        std::cout << "Active Owners after subscope thread call: " <<systemLogger.use_count() << std::endl;
    } // threadReference goes out of scope here. Count decrements to 1.
    std::cout << "\nActive Owners (Main Only): " << systemLogger.use_count() << std::endl;

    std::cout << "--- Shutting down system ---\n";
    systemLogger.reset(); // Manually dropping main's ownership. Count hits 0!
    std::cout << "System offline \n";

    std::cout << "================= SHARED POINTER EXAMPLE USE CASE II ===================" << std::endl;
    std::cout << "=== SYSTEM BOOT: IoT WEATHER SNAPSHOT RUN ===\n\n";

    {
        // 1. Thread spawns a new sensor snapshot
        std::shared_ptr<const WeatherSnapshot> latestReading = 
            std::make_shared<const WeatherSnapshot>(24.85f, 62.4f, 1012.3f, 45000ULL);

        std::cout << "Active Owners: " << latestReading.use_count() << "\n\n";

        // 2. Dispatch tasks (In an RTOS, this copies pointers into OS queues)
        // Here we simulate concurrent execution by passing the shared_ptr to threads
        std::thread t1(oledDisplayTask, latestReading);
        std::thread t2(sdLoggerTask, latestReading);
        std::thread t3(wifiPublisherTask, latestReading);

        // Main loop can immediately drop its local ownership to continue polling sensors
        latestReading.reset(); 
        std::cout << "[Main Thread] Dropped local owner pointer. System waiting for tasks...\n\n";

        t1.join();
        t2.join();
        t3.join();
    } 

    std::cout << "\n=== DIAGNOSTIC RUN ENDED ===" << std::endl;
    std::cout << "========================================================================" << std::endl;

    std::cout << "====================================================================\n" << std::endl;

    std::cout << "=================== WEAK POINTER EXAMPLE USE CASE ===================" << std::endl;
    std::cout << "--- Spawning Controller & Motor Loop ---\n";

    auto mainController = std::make_shared<Controller>();   //auto -> std::shared_ptr<Controller>
    auto rotor1 = std::make_shared<DroneMotor>();           //auto -> std::shared_ptr<DroneMotor>

    // Link them together
    mainController->m_MotorRef = rotor1;        // Stored inside a shared pointer
    rotor1->m_ControllerRef = mainController;   // Stored safely inside a Weak Pointer

    std::cout << "Main Controller reference count: " << mainController.use_count() << std::endl;
    std::cout << "Rotor Motor reference count: " << rotor1.use_count() << std::endl;

    // How to access a weak pointer
    // You cannot read it directly because the object might have already been deleted.
    // You must promote it temporarily to a shared_ptr using .lock()
    if(auto tempControllerShared = rotor1->m_ControllerRef.lock()){
        std::cout << "[Verification] Motor successfully verified its controller link \n";
    }

    std::cout << "================= WEAK POINTER EXAMPLE USE CASE II ===================" << std::endl;
    std::cout << "=== SYSTEM BOOT: DRONE TELEMETRY BROADCAST ===\n";

    SensorBroadcaster imuService;

    // Create our graphical screen widget on the stack
    auto oledScreen = std::make_shared<OledGraphicView>();

    // Register it as a weak observer so we don't hold it hostage in memory
    imuService.registerObserver(oledScreen);

    // Initial sensor sweep
    imuService.broadcast(1.2f, -0.4f);

    std::cout << "\n[Simulator] User closes the OLED screen widget...\n";
    // Deallocate the observer manually by releasing our owner pointer
    oledScreen.reset(); 

    // Second sensor sweep (Broadcaster detects the observer has departed!)
    imuService.broadcast(3.4f, -1.8f);

    std::cout << "\n=== SYSTEM SHUTDOWN ===" << std::endl;

    std::cout << "=====================================================================" << std::endl;
    
    std::cout << "--- Leaving Main Scope ---\n";
    std::cout << "=====================================================================" << std::endl;
    std::cin.get();
}