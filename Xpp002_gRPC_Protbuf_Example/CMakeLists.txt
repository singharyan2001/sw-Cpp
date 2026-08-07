cmake_minimum_required(VERSION 3.15)
project(SystemManager CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the required gRPC and Protbuf packages
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 2. Define paths
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/sysmanager.proto)

# Set the output directory for generated files INSIDE the build folder
# set(PROTO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
# file(MAKE_DIRECTORY ${PROTO_GEN_DIR})
set(PROTO_GEN_DIR_CPP ${CMAKE_CURRENT_BINARY_DIR}/generated/cpp)
set(PROTO_GEN_DIR_PY ${CMAKE_CURRENT_BINARY_DIR}/generated/python)
set(PROTO_GEN_DIR_GO ${CMAKE_CURRENT_BINARY_DIR}/generated/go)

file(MAKE_DIRECTORY ${PROTO_GEN_DIR_CPP})
file(MAKE_DIRECTORY ${PROTO_GEN_DIR_PY})
file(MAKE_DIRECTORY ${PROTO_GEN_DIR_GO})

# Locate the Python gRPC Plugin (Required because it's not a standard CMake target like C++)
find_program(GRPC_PYTHON_PLUGIN grpc_python_plugin REQUIRED)

# Manually point to the Go plugins in your home directory
set(GRPC_GO_PLUGIN $ENV{HOME}/go/bin/protoc-gen-go)
set(GRPC_GO_GRPC_PLUGIN $ENV{HOME}/go/bin/protoc-gen-go-grpc)
# find_program(GRPC_GO_PLUGIN protoc-gen-go PATHS $ENV{HOME}/go/bin REQUIRED)
# find_program(GRPC_GO_GRPC_PLUGIN protoc-gen-go-grpc PATHS $ENV{HOME}/go/bin REQUIRED)

# 3. Custom command to run the protocol buffer compiler (protoc)
# This generates the .pb.cc/.h and .grpc.pb.cc/.h files
add_custom_command(
    OUTPUT  ${PROTO_GEN_DIR_CPP}/sysmanager.pb.cc
            ${PROTO_GEN_DIR_CPP}/sysmanager.pb.h
            ${PROTO_GEN_DIR_CPP}/sysmanager.grpc.pb.cc
            ${PROTO_GEN_DIR_CPP}/sysmanager.grpc.pb.h
    COMMAND protobuf::protoc
    ARGS 
        # 1. C++ Outputs and Plugin
        --cpp_out=${PROTO_GEN_DIR_CPP}
        --grpc_out=${PROTO_GEN_DIR_CPP}
        --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>

        # 2. Python Outputs and Plugin
        --python_out=${PROTO_GEN_DIR_PY}
        --grpc_python_out=${PROTO_GEN_DIR_PY}
        --plugin=protoc-gen-grpc_python=${GRPC_PYTHON_PLUGIN}

        # 3. Go Outputs (Go plugins are usually found automatically if on path)
        --go_out=${PROTO_GEN_DIR_GO}
        --go_opt=paths=source_relative
        --plugin=protoc-gen-go=${GRPC_GO_PLUGIN}

        --go-grpc_out=${PROTO_GEN_DIR_GO}
        --go-grpc_opt=paths=source_relative
        --plugin=protoc-gen-go=${GRPC_GO_PLUGIN}

        # Input Definitions
        -I ${PROTO_DIR}
        ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}   
)

# Create a library target from the generated files
add_library(sysmanager_proto_lib
    ${PROTO_GEN_DIR_CPP}/sysmanager.pb.cc
    ${PROTO_GEN_DIR_CPP}/sysmanager.grpc.pb.cc
)

# Link the generated library against Protobuf and gRPC
target_link_libraries(sysmanager_proto_lib PUBLIC gRPC::grpc++ protobuf::libprotobuf)
target_include_directories(sysmanager_proto_lib PUBLIC ${PROTO_GEN_DIR_CPP})

# 4. Build the Server Executable
add_executable(sysmanager_server src/server.cpp)
target_link_libraries(sysmanager_server PRIVATE sysmanager_proto_lib)
target_include_directories(sysmanager_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${PROTO_GEN_DIR_CPP})
add_dependencies(sysmanager_server sysmanager_proto_lib)

# 5. Build the Client Executable
add_executable(sysmanager_client src/client.cpp)
target_link_libraries(sysmanager_client PRIVATE sysmanager_proto_lib)
target_include_directories(sysmanager_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${PROTO_GEN_DIR_CPP})
add_dependencies(sysmanager_client sysmanager_proto_lib)
