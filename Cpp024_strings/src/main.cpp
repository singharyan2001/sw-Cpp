#include <iostream>
#include <string>

void printLog(const std::string& msg);
void newPrintLog(std::string_view msg);

int main() {
	std::cout << "========== Topic: C++ Strings ==========" << std::endl;
	// Defining a raw string in C++
	const char* rawString = "Hello";
	std::cout << rawString << std::endl;

	// Calculating the size of a raw string
	int count = 0;
	for(int i=0; rawString[i] != '\0'; i++){
		std::cout << "Character: " << rawString[i] << std::endl;
		count++;
	}
	std::cout << "Size of the Raw String: " << count << std::endl;

	// Defining using the concrete type "string" in C++ using std::string
	std::string name = "Cherno";
	std::cout << name << std::endl;
	std::cout << "The size of the string (name): " << name.size() << std::endl;

	// String Operations & Concatenation
	std::string part1 = "The ";
	std::string part2 = "Cherno";
	// Using the overloaded operator +
	std::string full = part1 + part2;
	full += "!";
	// Modifies
	std::cout << full << std::endl;

	full += " >> C++ Game Developer @ EAGames";

	// Passing Strings to functions
	printLog(full); // Pass by const reference

	// The Modern C++17 Way: std::string_view
	full += " & Game Engine Designer";
	newPrintLog(full);

	newPrintLog("MODERN C++ STRING"); // Passing raw strings

	std::string team = "MUN UTD";
	newPrintLog(team);

	// Common String Methods
	std::string text = "Firmware Engine";

	// 1. Query properties
	int len = text.size();		// Returns the length of the string (excluding null terminator)
	bool empty = text.empty();	// Returns true if length is 0

	std::cout << "Length: " << len << ", isEmpty: " << empty << std::endl;

	// 2. Searching
	int pos = text.find("ware");	// Returns the starting index (4) or std::string::npos if not found

	std::cout << "Match Found? if yes, starting index pos: " << pos << std::endl;

	// 3. Substrings
	std::string sub = text.substr(0,  8);	// Retuns "Firmware" (Creates a new string)

	std::cout << "Sub String: " << sub << std::endl;

	// 4. Accessing raw C-Style pointer
	const char* rawPtr = text.c_str();	// Returns the underlying const char* null-terminated pointer. Crucial when passing C++ strings to C driver libraries!

	std::cout << "Accessing rawPtr pointer and passing to cout: " << rawPtr << std::endl;

	std::cout << "========================================" << std::endl;
//	std::cin.get();
	return 0;
}

// Traditional Pattern in
void printLog(const std::string& msg){
	std::cout << msg << std::endl;
}

void newPrintLog(std::string_view msg){
	std::cout << msg << std::endl;
}
