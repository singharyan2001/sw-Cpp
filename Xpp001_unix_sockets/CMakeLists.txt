# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(Xpp001_unix_sockets LANGUAGES CXX)

# Enforce Modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set strict compiler warnings (crucial for catching memory/pointer issues in POSIX API)
set(COMPILER_WARNINGS 
    -Wall 
    -Wextra 
    -Wpedantic
)

# ------------------------------------------------------------------------------
# Target: Server
# ------------------------------------------------------------------------------
add_executable(socket_server src/server.cpp)
target_compile_options(socket_server PRIVATE ${COMPILER_WARNINGS})
# Note: No target_link_libraries needed for standard POSIX sockets on Linux

# ------------------------------------------------------------------------------
# Target: Client
# ------------------------------------------------------------------------------
add_executable(socket_client src/client.cpp)
target_compile_options(socket_client PRIVATE ${COMPILER_WARNINGS})
