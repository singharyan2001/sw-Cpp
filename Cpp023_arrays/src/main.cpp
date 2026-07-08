// Arrays in C++
#include <iostream>
#include <array>

void processModern(const std::array<int, 5>& arr);
void stackExample();
void heapExample();

template <size_t N>
//void printRawArray(int (&arr)[N]);
void printRawArray(int (&arr)[N]){
	// N is resolved at compile time based on the array passed
	for (size_t i = 0; i < N; i++){
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::cout << "========== TOPIC: ARRAYS IN C++ ==========" << std::endl;

	// Two primary ways to declare an array
	int raw_array[5] = {1, 2, 3, 4, 5};
	std::array<int, 5> modern_array = {2, 4, 6, 8, 10};

	std::cout << "Size of the raw array of type int: " << sizeof(raw_array)/sizeof(int) << std::endl;
	std::cout << "Size of the raw array of type std::array: " << modern_array.size() << std::endl;


	// Stack & Heap Examples with array
	stackExample();
	heapExample();

	// Size Tracking and Safety for traditional C++ way to pass raw arrays
	int myBuffer[12] = {0};
	printRawArray(myBuffer); // Compiler automatically deduces N = 12

	// Size Tracking and Safety for the modern standard way of passing arrays
	processModern(modern_array);							// EXAMPLE 1
	std::array<int, 4> myBank = {10, 20, 30, 40};			// EXAMPLE 2
	std::cout << "Elements: " << myBank.size() << "\n";

	// Memory Access Violations & Bounds Checking
	int raw[3] = {1,2,3};
	raw[5] = 999; 						// CRITICAL BUG: Out of bounds write!!
	// It will print because raw is a pointer to the base address of the array, so raw array (5 * 4) times will give you the correct address to print 999,
	// But this is an out of bounds array access which is dangerous for any firmware or applications and will create bugs or could also crash a firmware
	std::cout << "Raw Array[5]: " << raw[5] << std::endl;

	// How std::array gives you the best of both worlds
	std::array<int, 3> new_array = {1,2,3};
	// method 1: subscript operator []
	new_array[5] = 999;
	std::cout << "new array[5]: " << new_array[5] << " " << "Out of bounds access was executed, this could create bugs or crash the system if the firmware relies on this array" << std::endl;
	// FAST! No bounds checking is performed at runtime.
	// WARNING! just as dangerous as a raw array if you go out of bounds.

	// method 2: .at() Method
	try {
		new_array.at(5) = 999;
	} catch (const std::out_of_range& e) {
		std::cout << "Safely caught error: " << e.what() << "\n";
	}
	// Safe! performs runtime bounds-checking.
	// If out of bounds, it throws a standard C++ exception

	std::cout << "==========================================" << std::endl;
//	std::cin.get();
	return 0;
}

// In a raw array, size information is lost upon passing!
void processRaw(int* arr) {
    // How big is arr? We have no idea unless we pass a separate size argument.
    // sizeof(arr) returns 8 (the size of a 64-bit pointer address), NOT the array size!
}

// In std::array, the size is baked directly into the TYPE Signature!
void processModern(const std::array<int, 5>& arr){
	// The compiler gurantees this array has exactly 5 elements.
	size_t length = arr.size(); // Compile-time query!
	std::cout << "Size of the given array: " << length << std::endl;
}

// 1. Stack Allocation
void stackExample(){
	std::array<int, 100> stackArr; // Created instantly via Stack Pointer math
	stackArr[0] = 77;
} // automatically destroyed when stackExample exits. Zero chances of a leak

void heapExample(){
	// Requesting 400 bytes from the operating system
	int* heapArr = new int[100];
	// Use it ...
	heapArr[0] = 42;
	// Mandatory manual cleanup
	delete[] heapArr;
} // If you forget delete[], you leak 400 bytes of RAM

