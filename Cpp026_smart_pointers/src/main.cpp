// Smart Pointers in C++
#include <iostream>
#include <memory>

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

//============================== Unique Pointer Concept ========================================

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

    std::cout << "--- Leaving Main Scope ---\n";

    std::cout << "=====================================================================" << std::endl;
    std::cin.get();
}