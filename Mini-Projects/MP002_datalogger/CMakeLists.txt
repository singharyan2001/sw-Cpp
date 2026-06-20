# 1. Grab all .cpp files in the src/ directory
file(GLOB SOURCES "src/*.cpp")

# 2. Define the executable using those source files
add_executable(MP002_datalogger ${SOURCES})

# 3. Tell the compiler where your header files (.h / .hpp) live
target_include_directories(MP002_datalogger PRIVATE inc)

# 4. Route the executable back to the HelloWorld folder
set_target_properties(MP002_datalogger PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})