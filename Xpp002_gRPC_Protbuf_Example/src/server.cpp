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
            std::cout << " -> Received chunk " << chunk.chunk_index() << " (" << chunk.data_payload().length() << " bytes" << std::endl;
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