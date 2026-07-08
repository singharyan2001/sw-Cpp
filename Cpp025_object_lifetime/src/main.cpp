// object lifetime in C++
#include <iostream>

#include <chrono>
#include <thread> // Simulating hardware delay

//========================= RAII EXAMPLE - THE SCOPED POINTER POINTER ================================

class IntBuffer {
public:
    IntBuffer(){
        std::cout << "[ALLOC] IntBuffer allocated on Heap." << std::endl;
    }

    ~IntBuffer(){
        std::cout << "[FREE] IntBuffer deleted from Heap." << std::endl;
    }
    void doWork(){
        std::cout << " -> IntBuffer processing sensor telemety...\n";
    }
};

// our custom scoped pointer wrapper
class ScopedIntBuffer{
private:
    IntBuffer *m_RawPointer; // The resource being wrapped
public:
    // Constructor: we take ownership of a raw heap pointer
    explicit ScopedIntBuffer(IntBuffer* rawPtr): m_RawPointer(rawPtr) {};

    // Destructor: Frees the pointer automatically on scope exit!
    ~ScopedIntBuffer(){
        delete m_RawPointer;
    }

    // Overload the arrow operator to let users use this wrapper like a pointer
    IntBuffer* operator->() {
        return m_RawPointer;
    }
};

void runTelemetryRoutine();

//====================================================================================================

//============= RAII EXAMPLE - SCOPED TELEMETRY PATTERN: AUTOMATIC TIMING PROFILER ===================

class ScopedTimer{
private:
    const char* m_TaskName;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
public:
    // Start timing at birth (Instantiation)
    explicit ScopedTimer(const char* taskName) : m_TaskName(taskName) {
        m_StartTime = std::chrono::high_resolution_clock::now();
        std::cout << "[PROFILE START] Working on task: " << m_TaskName << "\n";
    }

    // Stop timing and print logs at death (Destruction)
    ~ScopedTimer() {
        auto endTime = std::chrono::high_resolution_clock::now();
        
        // Calculate duration in microseconds
        auto startUs = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
        auto endUs = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
        auto durationUs = endUs - startUs;
        double durationMs = durationUs * 0.001; // convert to milliseconds

        std::cout << "[PROFILE END] " << m_TaskName << " finished in " 
                  << durationMs << " ms (" << durationUs << " us)\n\n";
    }
};


void runFftAnalysis();

//====================================================================================================

int main(){
    std::cout << "========== TOPIC: OBJECT LIFETIME IN C++ ==========" << std::endl;
    runTelemetryRoutine();

    std::cout << "--- SYSTEM DIAGNOSTIC RUN ---\n\n";
    
    runFftAnalysis();
    
    // We can also profile smaller internal blocks of code using nested braces {}
    {
        ScopedTimer dbTimer("Flash Write Verification");
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }

    std::cout << "--- DIAGNOSTIC COMPLETE ---\n";
    
    std::cout << "===================================================" << std::endl;
    std::cin.get();
}

void runTelemetryRoutine(){
    std::cout << "Entering runTelemetryRoutine()\n";

    // Instantiating our scoped wrapper on the STACK.
    // In its constructor, it captures a newly created IntBuffer sitting on the HEAP.
    ScopedIntBuffer sensorData(new IntBuffer());

    sensorData->doWork();

    std::cout << "Exiting runTelementryRoutine()\n";
    // Automatic Stack Cleanup:
    // 'sensorData' falls out of scope here.
    // The compiler automatically calls ~ScopedIntBuffer().
    // Inside that destructor, 'delete m_RawPointer' is executed.
    // The heap memory is freed with ZERO manual delete statements!
}


void runFftAnalysis() {
    // 1. Instantiating the ScopedTimer at the top of the function
    ScopedTimer timer("FFT Hardware Compute Loop");

    // 2. Simulating a heavy computation delay
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}