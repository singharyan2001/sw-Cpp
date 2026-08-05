// client.cpp

#include <iostream>
#include <memory>
#include <string>

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
        
        grpc::Status status1 = stub_->GetSystemStatus(&context1, request1, &reply1);
        if(status1.ok()){
            std::cout << "Success! Online: " << reply1.is_online() << " | State Enum: " << reply1.current_state() << std::endl;
        }

        std::cout << "\n--- Testin Unary RPC 2 (TriggerEnclosureControl) ---\n";
        sysmanager::EnclosureControlRequest request2;
        request2.set_action(1); // OPEN
        sysmanager::EnclosureControlResponse reply2;
        grpc::ClientContext context2;

        grpc::Status status2 = stub_->TriggerEnclosureControl(&context2, request2, &reply2);
        if(status2.ok()){
            std::cout << "Success! Status Code: " << reply2.status() << std::endl;
        }
    }

    // =================================================================
    // 2. SERVER STREAMING RPC
    // =================================================================
};

void RunClient(){
    // Create a channel connecting to the Unix Domain Socket
    std::string target_uds = "unix:///tmp/sysmanager_grpc.sock";

    // Instantiate the client with an insecure channel
    SysManagerClient client(grpc::CreateChannel(target_uds, grpc::InsecureChannelCredentials()));

    std::cout << "Connecting to gRPC server at: " << target_uds << "..." << std::endl;

    // Test 1: Recognized Component
    client.RequestStatus("power_board");

    // Test 2: Unrecognized Component
    client.RequestStatus("cooling_fan");
}

int main(){
    std::cout << "========== gRPC Client ==========\n";

    RunClient();

    return 0;
}