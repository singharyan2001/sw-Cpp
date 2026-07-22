# 1. Grab all .cpp files in the src/ directory
file(GLOB SOURCES "src/*.cpp")

# 2. Define the executable using those source files
add_executable(Cpp033_lvalues_rvalues ${SOURCES})

# 3. Tell the compiler where your header files (.h / .hpp) live
target_include_directories(Cpp033_lvalues_rvalues PRIVATE inc)

# 4. Route the executable back to the folder
set_target_properties(Cpp033_lvalues_rvalues PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})