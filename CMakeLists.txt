cmake_minimum_required(VERSION 3.20)

project(Cpp-Workspace CXX)

# Enforce modern standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Cpp001_helloworld)
# add_subdirectory()
# add_subdirectory()
add_subdirectory(Cpp004_references)
