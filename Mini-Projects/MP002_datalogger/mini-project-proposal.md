# Mini-Project 002: Sensor Data Logger

## Project Overview
Building on the mechanics of the Ring Buffer, the Data Logger simulates a real-world asynchronous firmware environment. It involves handling custom data payloads (structs) instead of primitive integers, simulating a hardware polling routine, and batch-processing the collected data.

## Core Objectives
1. **Struct Composition:** Learn to handle nested or complex structs (a buffer that holds other structs).
2. **Pass-by-Const-Reference:** Reinforce the habit of passing large data payloads as `const Type&` to avoid expensive memory copying while maintaining read-only safety.
3. **Simulation Logic:** Write mock "Producer" and "Consumer" functions to simulate hardware interrupts and main-loop processing.

## Technical Requirements
1. **Language:** C++17 (Procedural only).
2. **Dependencies:** Must reuse or adapt the logic from Mini-Project 1, but modified to hold a custom struct instead of integers.
3. **Build System:** CMake (Integrated into the Master Workspace as MP002_datalogger).

## Functional Specifications
- `struct SensorPayload`
    - Contains: `uint32_t timestamp`, `float temperature`, `uint8_t sensor_id`.
- `struct LoggerBuffer`
    - Contains: An array of SensorPayload structs, plus the standard head/tail/count trackers.
- `void generateMockData(SensorPayload& payload, uint32_t current_time)`
    - Simulates a hardware read. Populates the referenced payload with dummy temperature data and the current time.
- `void logData(LoggerBuffer& buffer, const SensorPayload& payload)`
    - Acts as the "Producer". Pushes the structured payload into the buffer. Fails safely if full. Note the use of `const &` for the payload to prevent copying.
- `void flushLogger(LoggerBuffer& buffer)`
    - Acts as the "Consumer". Loops through the buffer, pops every available SensorPayload, and formats it beautifully to `std::cout` using C++ streams.

## Testing & Validation (In `main.cpp`)
Write a simulated runtime loop:
1. Create a `for` loop that runs 20 times (simulating 20 seconds of runtime).
2. Simulation: Every 1 loop iteration, call `generateMockData` and `logData` to simulate a fast sensor.
3. Simulation: Every 5 loop iterations, call `flushLogger` to simulate a slower, delayed task that clears out the buffer.
4. Verify that the timestamps print chronologically and that no data is dropped or corrupted during the wrap-around phase of the ring buffer.

---