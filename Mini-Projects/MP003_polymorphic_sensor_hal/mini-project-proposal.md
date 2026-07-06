# Mini-Project 3: Polymorphic Sensor HAL

## Project Overview
In modern embedded systems, high-level application logic (like a weather station or flight controller) should never talk directly to hardware registers. It should talk to an Interface.

This project requires you to design a pure abstract interface for a generic sensor, implement two specific hardware sensors that satisfy that interface, and write a main loop that reads from both sensors interchangeably using Polymorphism (Dynamic Dispatch).

## Core Objectives
1. **Interface Design:** Define a strict contract using pure virtual functions (`= 0`).
2. **Inheritance & Overriding:** Create concrete sub-classes that implement the interface using the override keyword.
3. **Polymorphism:** Use an array of Base Class pointers to manipulate Derived Class objects without knowing their true types.

## Technical Requirements
1. **Language:** C++17 (Strictly Object-Oriented).
2. **Build System:** CMake (Integrated into the Master Workspace as 03_sensor_hal).
3. **Files:** Split into ISensor.h, TempSensor.h/cpp, PressureSensor.h/cpp, and main.cpp.

## Functional Specifications
**Part A: The Interface (`ISensor.h`):** Create an Interface class named `ISensor`.
1. It must have a pure virtual function: `virtual bool init() = 0;`
2. It must have a pure virtual fucntion: `virtual float readValue() = 0;`
2. It must have a virtual destructor: `Virtual ~ISensor() = default;`

**Part B: Concrete Class 1 (`TempSensor`):** Create a class that inherits from `ISensor`.
1. Private State: A mock memory address (e.g., int i2c_address) initialized via a Constructor using a Member Initializer List.
2. Override `init()`: Print "Initializing Temp Sensor at I2C address: [addr]" and return true.
3. Override `readValue()`: Return a mock temperature (e.g., 24.5).

**Part C: Concrete Class 2 (`PressureSensor`):** Create a class that inherits from ISensor.
1. Private State: A mock SPI pin (e.g., int spi_cs_pin) initialized via a Constructor using a Member Initializer List.
2. Override init(): Print "Initializing Pressure Sensor on SPI CS Pin: [pin]" and return true.
3. Override readValue(): Return a mock pressure (e.g., 1013.25).

## Testing & Validation (In `main.cpp`)
Your `main.cpp` must prove that dynamic dispatch is working:
1. Instantiate one `TempSensor` and one `PressureSensor` on the Stack.
2. Create an array of `ISensor*` (Base pointers) of size 2.
3. Store the addresses of your two concrete sensors in that array.
4. The Polymorphic Loop: Write a for loop that iterates through the array. For each pointer:
    1. `Call ->init()`
    2. `Call ->readValue()`
5. Print the returned value to the console.

**Success Criteria:** The for loop should execute the correct derived methods automatically, proving that the VTable successfully mapped the generic `ISensor*` pointers to the correct hardware logic at runtime.

---
