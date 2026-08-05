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
set(PROTO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

# 3. Custom command to run the protocol buffer compiler (protoc)
# This generates the .pb.cc/.h and .grpc.pb.cc/.h files
add_custom_command(
    OUTPUT  ${PROTO_GEN_DIR}/sysmanager.pb.cc
            ${PROTO_GEN_DIR}/sysmanager.pb.h
            ${PROTO_GEN_DIR}/sysmanager.grpc.pb.cc
            ${PROTO_GEN_DIR}/sysmanager.grpc.pb.h
    COMMAND protobuf::protoc
    ARGS --cpp_out=${PROTO_GEN_DIR}
         --grpc_out=${PROTO_GEN_DIR}
         --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
         -I ${PROTO_DIR}
          ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}   
)

# Create a library target from the generated files
add_library(sysmanager_proto_lib
    ${PROTO_GEN_DIR}/sysmanager.pb.cc
    ${PROTO_GEN_DIR}/sysmanager.grpc.pb.cc
)

# Link the generated library against Protobuf and gRPC
target_link_libraries(sysmanager_proto_lib PUBLIC gRPC::grpc++ protobuf::libprotobuf)
target_include_directories(sysmanager_proto_lib PUBLIC ${PROTO_GEN_DIR})

# 4. Build the Server Executable
add_executable(sysmanager_server src/server.cpp)
target_link_libraries(sysmanager_server PRIVATE sysmanager_proto_lib)
target_include_directories(sysmanager_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${PROTO_GEN_DIR})
add_dependencies(sysmanager_server sysmanager_proto_lib)

# 5. Build the Client Executable
add_executable(sysmanager_client src/client.cpp)
target_link_libraries(sysmanager_client PRIVATE sysmanager_proto_lib)
target_include_directories(sysmanager_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${PROTO_GEN_DIR})
add_dependencies(sysmanager_client sysmanager_proto_lib)
