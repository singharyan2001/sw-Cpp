cmake_minimum_required(VERSION 3.20)

project(Cpp-Workspace CXX)

# Enforce modern standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Cpp001_helloworld)
# add_subdirectory()
# add_subdirectory()
add_subdirectory(Cpp004_references)
add_subdirectory(Cpp005_classes)
add_subdirectory(Cpp006_static)
add_subdirectory(Cpp007_enums)
add_subdirectory(Cpp009_inheritance)
add_subdirectory(Cpp010_virtual_functions)
add_subdirectory(Cpp011_interfaces)
add_subdirectory(Cpp012_visibility)

add_subdirectory(Mini-Projects/MP001_ring_buffer)
add_subdirectory(Mini-Projects/MP002_datalogger)
