# GUIDE 01: Basics of gRPC & Protobuf in C++

This guide covers configuring VS Code, defining your Protocol Buffers, setting up an automated CMake build pipeline, and implementing all four gRPC communication primitives (Unary, Server Streaming, Client Streaming, and Bidirectional Streaming) over Unix Domain Sockets.

**Note:** The Basic examples will be covered in a single thread based execution, In the next guide we will switch from blocking & sequential execution to concurrency based workflows.

## Directory Architecture
Your project must be organized cleanly to separate raw source code from auto-generated files.

Create the following structure in your workspace:
```text
dock-comstack/
├── CMakeLists.txt      # The build configuration
├── proto/              
│   └── sysmanager.proto # Your Protocol Buffer definitions
├── inc/                # Custom C++ header files
├── src/                
│   ├── server.cpp      # gRPC Server implementation
│   └── client.cpp      # gRPC Client implementation
└── build/              # All compiled binaries and generated headers go here!

```

## Defining the Data Model (`proto/sysmanager.proto`)
Here we define exactly how our data is structured, including the Enums for state management, and all four types of RPC streams.

```proto
// Specify the syntax version
syntax = "proto3";

// Create a namespace for generated C++ classes
package sysmanager;

// =====================================================================
// ENUMS
// =====================================================================
enum SystemState {
    SYSTEM_STATE_UNSPECIFIED = 0;
    SYSTEM_STATE_BOOTING = 1;
    SYSTEM_STATE_IDLE = 2;
    SYSTEM_STATE_ACTIVE = 3;
    SYSTEM_STATE_FAULT = 4;
}

// =====================================================================
// 1. UNARY MESSAGES
// =====================================================================
message StatusRequest{
    string component_name = 1;
}
message StatusResponse{
    bool is_online = 1;
    SystemState current_state = 2;
}

message EnclosureControlRequest{
    int32 action = 1; // e.g., 1 = Lock, 2 = Unlock
}
message EnclosureControlResponse{
    int32 status = 1; 
}

// =====================================================================
// 2. SERVER STREAMING MESSAGES (Telemetry)
// =====================================================================
message WeatherRequest {
    int32 frequency_ms = 1;
}
message WeatherTelemetry {
    float wind_speed = 1;
    float temperature = 2;
}

// =====================================================================
// 3. CLIENT STREAMING MESSAGES (Firmware Upload)
// =====================================================================
message FirmwareChunk {
    int32 chunk_index = 1;
    string data_payload = 2; // Simulated byte array
}
message UploadResult {
    int32 total_bytes_received = 1;
}

// =====================================================================
// 4. BIDIRECTIONAL STREAMING MESSAGES (Control Loop)
// =====================================================================
message MotorCommand {
    float target_position = 1;
}
message MotorTelemetry {
    float current_position = 1;
    bool is_moving = 2;
}

// =====================================================================
// SERVICE DEFINITION
// =====================================================================
service SystemManager{
    // 1. Unary RPCs
    rpc GetSystemStatus (StatusRequest) returns (StatusResponse);
    rpc TriggerEnclosureControl (EnclosureControlRequest) returns (EnclosureControlResponse);
    
    // 2. Server Streaming (Returns a stream of data)
    rpc StreamWeatherTelemetry (WeatherRequest) returns (stream WeatherTelemetry);
    
    // 3. Client Streaming (Accepts a stream of data)
    rpc UploadFirmware (stream FirmwareChunk) returns (UploadResult);
    
    // 4. Bidirectional Streaming (Both sides stream)
    rpc ActiveMotorControl (stream MotorCommand) returns (stream MotorTelemetry);
}
```

## The Build Pipeline (`CMakeLists.txt`)

```cmake
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
```

## Demystifying the Generated Code
When you run CMake, gRPC generates your base classes. Here is the strict mapping rule you use to know what to write:
1. `message StatusRequest` -> Becomes a C++ data class: `sysmanager::StatusRequest`
2. `service SystemManager` -> Generates the Server base class: `sysmanager::SystemManager::Service`
3. **Stream Mapping:**
    1. Unary outputs: `sysmanager::StatusResponse* reply`
    2. Server Streams: `grpc::ServerWriter<sysmanager::WeatherTelemetry>* writer`
    3. Client Streams: `grpc::ServerReader<sysmanager::FirmwareChunk>* reader`
    4. BiDi Streams: `grpc::ServerReaderWriter<sysmanager::MotorTelemetry, sysmanager::MotorCommand>* stream`

## Implementing the Server (`src/server.cpp`)

Here we inherit from the generated Service class and implement all four primitives.

```cpp
// server.cpp

#include <iostream>
#include <memory>
#include <string>

#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include "sysmanager.grpc.pb.h" // need to include the header in the build directory

// Inherit from the generated service base class using the predicted mapping
class SystemManagerServiceImpl final : public sysmanager::SystemManager::Service {
    // =================================================================
    // 1. UNARY RPCs
    // =================================================================
    // Override the Unary RPC defined in the .proto file
    grpc::Status GetSystemStatus(
        grpc::ServerContext* context,
        const sysmanager::StatusRequest* request,
        sysmanager::StatusResponse* reply
    ) override
    {
        std::cout << "[SYSTEM STATUS] Received request for component: " << request->component_name() << std::endl;
        // Dummy Logic based on the request
        if(request->component_name() == "power_board"){
            reply->set_is_online(true);
            reply->set_current_state("Nominal Voltage");
        }
        else if(request->component_name() == "dock_core"){
            reply->set_is_online(true);
            reply->set_current_state("OPERATIONAL");
        }
        else{
            reply->set_is_online(false);
            reply->set_current_state("Component not found or offline");
        }
        // Return an OK Status to the Client
        return grpc::Status::OK;
    }

    grpc::Status TriggerEnclosureControl(
        grpc::ServerContext* context,
        const sysmanager::EnclosureControlRequest* request,
        sysmanager::EnclosureControlResponse* reply
    ) override {
        // LOG REQUEST
        std::cout << "[ENCLOSURE CONTROL] Recieved trigger request for enclosure door control: " << request->action() << std::endl;
        
        // PROCESS REQUEST
        int32_t control_request = request->action();
        if(control_request == 1){
            std::cout << "{ACTION} Triggered enclosure door open\n";
            reply->set_status(0);
        }
        else if(control_request ==2){
            std::cout << "{ACTION} Triggered enclosure door close\n";
            reply->set_status(0);
        }
        else{
            std::cout << "{INVALID} Unknown request received for enclosure door control\n";
            reply->set_status(-1);
        }

        // Return
        return grpc::Status::OK;
    }

    // =================================================================
    // 2. SERVER STREAMING RPCs
    // =================================================================
    grpc::Status StreamWeatherTelemetry(
        grpc::ServerContext* context,
        const sysmanager::WeatherRequest* request,
        grpc::ServerWriter<sysmanager::WeatherTelemetry>* writer
    ) override {
        std::cout << "[SERVER STREAM] Starting Weather Telemetry at " << request->frequency_ms() << "ms interval" << std::endl;

        // PUSH X Updates down the pipe, then cleanly exit
        for(int i = 0 ; i < 10; i++){
            if(context->IsCancelled()){
                break;  // Stop if client disconnected
            }

            sysmanager::WeatherTelemetry telemetry;
            telemetry.set_wind_speed(10.5f + i);
            telemetry.set_temperature(22.0f);

            writer->Write(telemetry);
            std::this_thread::sleep_for(std::chrono::milliseconds(request->frequency_ms()));
        }

        std::cout << "[SERVER STREAM] Telemetry Stream finished." << std::endl;
        return grpc::Status::OK;
    }

    // =================================================================
    // 3. CLIENT STREAMING RPCs
    // =================================================================
    grpc::Status UploadFirmware(
        grpc::ServerContext* context,
        grpc::ServerReader<sysmanager::FirmwareChunk>* reader,
        sysmanager::UploadResult* reply
    ) override {
        std::cout << "[CLIENT STREAM] Receiving firmware ..." << std::endl;

        sysmanager::FirmwareChunk chunk;
        int total_bytes = 0;

        // The Read() loop blocks until the client sends data or closes the stream
        while(reader->Read(&chunk)){
            total_bytes += chunk.data_payload().length();
            std::cout << " -> Received chunk index: " << chunk.chunk_index() << "| Payload length: " << chunk.data_payload().length() << " bytes" << std::endl;
        }

        // Reply once at the very end
        reply->set_total_bytes_received(total_bytes);
        std::cout << "[CLIENT STREAM] Upload complete. Total Bytes: " << total_bytes << std::endl;

        return grpc::Status::OK;
    }

    // =================================================================
    // 4. BIDIRECTIONAL STREAMING RPCs
    // =================================================================
    grpc::Status ActiveMotorControl(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<sysmanager::MotorTelemetry, sysmanager::MotorCommand>* stream
    ) override {
        std::cout << "[BiDi] Motor control loop started." << std::endl;

        sysmanager::MotorCommand command;
        // Server constantly reads from the client
        while(stream->Read(&command)){
            std::cout << " -> [BiDi RX] Command Target: " << command.target_position() << std::endl;

            // Server responds Instantly on the same pipe
            sysmanager::MotorTelemetry telemetry;
            telemetry.set_current_position(command.target_position());
            telemetry.set_is_moving(false);
            stream->Write(telemetry);
            std::cout << " <- [BiDi TX] Telemetry Sent." << std::endl;
        }

        std::cout << "[BiDi STREAM] Control loop closed." << std::endl;
        return grpc::Status::OK;
    }
};


void RunServer(){
    // Define our Unix Domain Socket path
    std::string server_address("unix:///tmp/sysmanager_grpc.sock");

    SystemManagerServiceImpl service;
    grpc::ServerBuilder builder;

    // Tell gRPC to listen on our UDS without any encryption
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register our service logic
    builder.RegisterService(&service);

    // Register our server thread
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (server == nullptr) {
        std::cerr << "CRITICAL ERROR: Failed to start the gRPC server!" << std::endl;
        return; 
    }

    std::cout << "gRPC Server Listening on " << server_address << std::endl;

    // Block until the server shuts down
    server->Wait();
}

int main(){
    std::cout << "========== gRPC Server ==========\n";
    
    RunServer();

    return 0;
}
```

## Implementing the Client

Here, we will implement our wrapper class which calls the overrided stub methods:

```cpp
// client.cpp

#include <iostream>
#include <memory>
#include <string>

#include <thread>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include "sysmanager.grpc.pb.h"

class SysManagerClient{
private:
    // The Stub is our local proxy for the remote server
    std::unique_ptr<sysmanager::SystemManager::Stub> stub_;
public:
    // The Constuctor takes a channel, which represents the connection to the server.
    // Notice how we use sysmanager::SystemManager::NewStub here.
    SysManagerClient(std::shared_ptr<grpc::Channel> channel) 
        : stub_(sysmanager::SystemManager::NewStub(channel)) {}
    
    // A Wrapper function to call our Unary RPC
    void RequestStatus(const std::string& component){
        // Prepare the request payload
        sysmanager::StatusRequest request;
        request.set_component_name(component);

        // Prepare the response payload container
        sysmanager::StatusResponse reply;

        // Context can be used to set timeouts, headers, etc.
        grpc::ClientContext context;

        // Perform the actual RPC Call using the predicted method name
        grpc::Status status = stub_->GetSystemStatus(&context, request, &reply);

        // Check if the network/Socket communication succeeded
        if(status.ok()){
            std::cout << "\n--- Status Report for: " << component << " ---" << std::endl;
            std::cout << "Online: " << (reply.is_online() ? "Yes" : "No") << std::endl;
            std::cout << "Message: " << reply.current_state() << std::endl;
        }
        else {
            std::cout << "RPC failed. Error code: " << status.error_code()
                      << ", message: " << status.error_message() << std::endl;
        }
    }

    // =================================================================
    // 1. UNARY RPC
    // =================================================================
    void CallUnary(){
        std::cout << "\n--- Testing Unary RPC 1 (Get System Status) ---" << std::endl;
        sysmanager::StatusRequest request1;
        request1.set_component_name("dock_core");
        sysmanager::StatusResponse reply1;
        grpc::ClientContext context1;
        
        std::cout << "[UNARY RPC] Sending Request: Get System Status\n";
        grpc::Status status1 = stub_->GetSystemStatus(&context1, request1, &reply1);
        if(status1.ok()){
            std::cout << "Success! Online: " << reply1.is_online() << " | State Enum: " << reply1.current_state() << std::endl;
        }

        std::cout << "\n--- Testin Unary RPC 2 (TriggerEnclosureControl) ---\n";
        sysmanager::EnclosureControlRequest request2;
        request2.set_action(1); // OPEN
        sysmanager::EnclosureControlResponse reply2;
        grpc::ClientContext context2;

        std::cout << "[UNARY RPC] Sending Request: Trigger Enclosure Door Open\n";
        grpc::Status status2 = stub_->TriggerEnclosureControl(&context2, request2, &reply2);
        if(status2.ok()){
            std::cout << "Success! Status Code: " << reply2.status() << std::endl;
        }
    }

    // =================================================================
    // 2. SERVER STREAMING RPC
    // =================================================================
    void CallServerStreaming(){
        std::cout << "\n--- Testing Server Streaming (Weather Telemetry) ---\n";
        sysmanager::WeatherRequest request;
        request.set_frequency_ms(200);

        grpc::ClientContext context;
        sysmanager::WeatherTelemetry telemetry;

        std::cout << "[SERVER STREAMING RPC] Subscribing to Weather Telemetry Stream...\n";

        // Get the reader stream
        std::unique_ptr<grpc::ClientReader<sysmanager::WeatherTelemetry>> reader(stub_->StreamWeatherTelemetry(&context, request));

        // Block and read data as it arrives from the server
        while(reader->Read(&telemetry)){
            std::cout << " [STREAM RX] WindSpeed: " << telemetry.wind_speed() << " m/s | Temperature: " << telemetry.temperature() << "*C\n";
        }

        grpc::Status status = reader->Finish();
        if(status.ok()){
            std::cout << "Server Stream finished cleanly.\n";
        }
    }

    // =================================================================
    // 3. Client STREAMING RPC
    // =================================================================
    void CallClientStreaming(){
        std::cout << "\n--- Testing Client Streaming (Firmware Upload) ---\n";
        grpc::ClientContext context;
        sysmanager::UploadResult reply;

        std::cout << "[CLIENT STREAMING RPC] Publishing firmware file to server...\n";

        // Get the writer stream. pass the reply object to catch the final response.
        std::unique_ptr<grpc::ClientWriter<sysmanager::FirmwareChunk>> writer(stub_->UploadFirmware(&context, &reply));

        for(int i=0; i < 3 ; i++){
            sysmanager::FirmwareChunk chunk;
            chunk.set_chunk_index(i);
            chunk.set_data_payload("binary_data_chunk_" + std::to_string(i));

            std::cout << " [STREAM TX] Sending chunk " << i << "..." << std::endl;
            writer->Write(chunk);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // CRITICAL: Tell the server we are done writing
        writer->WritesDone();

        // Wait for the single response
        grpc::Status status = writer->Finish();
        if(status.ok()){
            std::cout << "Upload Success! Total bytes processed: " << reply.total_bytes_received() << std::endl;
        }
    }

    // =================================================================
    // 4. BIDIRECTIONAL STREAMING RPC
    // =================================================================
    void CallBidirectionalStreaming(){
        std::cout << "\n---Testing Bidirectional Streaming (Motor Control) ---\n";
        grpc::ClientContext context;

        std::cout << "[BIDIRECTIONAL STREAMING RPC] Subscribing to Motor Telemetry & Publishing Motor Commands\n";

        // Get the reader/writer stream
        std::shared_ptr<grpc::ClientReaderWriter<sysmanager::MotorCommand, sysmanager::MotorTelemetry>> stream(stub_->ActiveMotorControl(&context));

        // Ping-Pong Approach: Client writes, then immediately reads
        for(int i = 1; i <= 3; i++){
            sysmanager::MotorCommand command;
            command.set_target_position(10.5f + i);

            std::cout << " [BiDi TX] Sending Target Position: " << command.target_position() << std::endl;
            stream->Write(command);

            sysmanager::MotorTelemetry telemetry;
            if(stream->Read(&telemetry)){
                std::cout << "  [BiDi Rx] Telemetry - Pos: " << telemetry.current_position() << " | Moving: " << telemetry.is_moving() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(250));
        }

        stream->WritesDone();
        grpc::Status status = stream->Finish();
        if(status.ok()){
            std::cout << "Bidirectional Stream Finished.\n";
        }
    }
};

#define example 0

void RunClient(){
    // Create a channel connecting to the Unix Domain Socket
    std::string target_uds = "unix:///tmp/sysmanager_grpc.sock";

    // Instantiate the client with an insecure channel
    SysManagerClient client(grpc::CreateChannel(target_uds, grpc::InsecureChannelCredentials()));

    std::cout << "Connecting to gRPC server at: " << target_uds << "..." << std::endl;

    std::cout << "========== gRPC Client Started ==========\n";

#if example
    // Test 1: Recognized Component
    client.RequestStatus("power_board");

    // Test 2: Unrecognized Component
    client.RequestStatus("cooling_fan");
#else
    client.CallUnary();
    std::chrono::seconds(2);
    client.CallServerStreaming();
    std::chrono::seconds(2);
    client.CallClientStreaming();
    std::chrono::seconds(2);
    client.CallBidirectionalStreaming();
#endif

}

int main(){
    std::cout << "========== gRPC Client ==========\n";

    RunClient();

    return 0;
}
```
---