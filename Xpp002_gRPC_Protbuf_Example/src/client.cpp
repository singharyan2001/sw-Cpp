// client.cpp

#include <iostream>
#include <memory>
#include <string>

#include <thread>
#include <chrono>
#include <atomic>

#include <grpcpp/grpcpp.h>
#include "sysmanager.grpc.pb.h"


//===========================================================================================================================
// CLient Example Configuration
//===========================================================================================================================

/* Guide Config Fla
 * Guide Config Flag can be configured to select respective example to run
 * 1 -> GUIDE 01 Example configured
 * 3 -> GUIDE 03 Example configured
 * 2 -> GUIDE 02 Example configured
 */
#define GUIDE_CONFIG_FLAG       2

// GUIDE 01 related configuration
#define test_inital_example     0   // If 0, it will enable the 4 primitives example test, and if 1, it will enable the unary test example


#if (GUIDE_CONFIG_FLAG == 1)
//===========================================================================================================================
// GUIDE 01: Basics of gRPC & Protobuf with C++
//===========================================================================================================================

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

void RunClient(){
    // Create a channel connecting to the Unix Domain Socket
    std::string target_uds = "unix:///tmp/sysmanager_grpc.sock";

    // Instantiate the client with an insecure channel
    SysManagerClient client(grpc::CreateChannel(target_uds, grpc::InsecureChannelCredentials()));

    std::cout << "Connecting to gRPC server at: " << target_uds << "..." << std::endl;

    std::cout << "========== gRPC Client Started ==========\n";

#if test_inital_example
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

#elif (GUIDE_CONFIG_FLAG == 2)
//===========================================================================================================================
// GUIDE 02: Concurrency Workflows with gRPC & Protobuf in C++
//===========================================================================================================================
class RealTimeSysClient {
private:
    std::unique_ptr<sysmanager::SystemManager::Stub> stub_;
    std::atomic<bool> is_running_;

    // Threading Components
    std::thread weather_thread_;
    grpc::ClientContext weather_context_; // Persisted, so we can cancel it from Main Thread
public:
    RealTimeSysClient(std::shared_ptr<grpc::Channel> channel) : stub_(sysmanager::SystemManager::NewStub(channel)), is_running_(true) {}

    ~RealTimeSysClient(){
        StopAll();
    }

    // =================================================================
    // BACKGROUND TASK: Continuous Weather Telemetry
    // =================================================================
    void StartWeatherStream(){
        // Spawn a completely independent C++ thread
        weather_thread_ = std::thread([this]() {
            sysmanager::WeatherRequest request;
            request.set_frequency_ms(500);
            sysmanager::WeatherTelemetry telemetry;

            // Start the server stream
            std::unique_ptr<grpc::ClientReader<sysmanager::WeatherTelemetry>> reader(stub_->StreamWeatherTelemetry(&weather_context_, request));

            std::cout << "[Weather Thread] Listening for continuous telemetry..." << std::endl;

            // This blocks the BACKGROUND thread until data arrives, or until weather_context_.TryCancel() is called by the main thread.
            while(reader->Read(&telemetry)){
                std::cout << "[Weather] Wind: " << telemetry.wind_speed() << " m/s | Temperature: " << telemetry.temperature() << " *C\n";
            }

            std::cout << "[Weather Thread] Exited safely." << std::endl;
        });
    }

    // =================================================================
    // FOREGROUND TASK: Unary Command (Callable anytime)
    // =================================================================
    void FireDoorCommand(int action){
        std::cout << "\n--- [Main Thread] Firing Enclosure Command: " << action << std::endl;
        sysmanager::EnclosureControlRequest request;
        request.set_action(action);
        sysmanager::EnclosureControlResponse reply;
        grpc::ClientContext context;

        // This blocks only the MAIN thread for a microsecond.
        // It shares the exact same gRPC channel with the weather thread seamlessly!
        grpc::Status status = stub_->TriggerEnclosureControl(&context, request, &reply);
        if(status.ok()){
           std::cout << "[Main Thread] Enclosure Command Success!" << std::endl;
        } else {
            std::cout << "[Main Thread] Enclosure Command Failed!" << std::endl;
        }
    }

    // =================================================================
    // SHUTDOWN LOGIC
    // =================================================================
    void StopAll(){
        if(is_running_){
            is_running_ = false;
            std::cout << "\n[System] Shutting down concurrent streams..." << std::endl;
            
            // Canceling the context forces the background thread's Read() loop to break instantly
            weather_context_.TryCancel(); 

            // Wait for the thread to finish
            if(weather_thread_.joinable()){
                weather_thread_.join();
            }
            std::cout << "[System] Shutdown complete." << std::endl;
        }
    }
};

void RunClient(){
    //1. create a single, thread safe channel
    std::string target_str = "unix:///tmp/sysmanager_grpc.sock";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());

    RealTimeSysClient client(channel);
    std::cout << "========== gRPC Concurrent Client Started ==========\n";

    // 2. kick off the continous background streams
    client.StartWeatherStream();

    // 3. The main thread continues to execute completely independly!
    std::cout << "[Main Thread] Processing core logic while weather streams in background..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 4. Fire Unary Commands WHILE the weather stream is running on the other thread
    client.FireDoorCommand(1); // Door open
    std::this_thread::sleep_for(std::chrono::seconds(2));
    client.FireDoorCommand(2); // Door close
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 5. Clean up gracefully
    client.StopAll();
}

#elif (GUIDE_CONFIG_FLAG == 3)
//===========================================================================================================================
// GUIDE 03: Concurrency Workflows with gRPC & Protobuf in C++
//===========================================================================================================================


#endif

int main(){
    std::cout << "========== gRPC Client ==========\n";
    
    RunClient();

    return 0;
}