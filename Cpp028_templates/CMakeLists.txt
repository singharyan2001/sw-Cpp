# 1. Grab all .cpp files in the src/ directory
file(GLOB SOURCES "src/*.cpp")

# 2. Define the executable using those source files
add_executable(Cpp028_templates ${SOURCES})

# 3. Tell the compiler where your header files (.h / .hpp) live
target_include_directories(Cpp028_templates PRIVATE inc)

# 4. Route the executable back to the folder
set_target_properties(Cpp028_templates PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})