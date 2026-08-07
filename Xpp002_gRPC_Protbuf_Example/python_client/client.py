import sys
import os
import time
import grpc

# 1. Add the generated Python files to the system path so Python can find them
# This points to the build/generated/python directory
generated_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '../build/generated/python'))
sys.path.append(generated_dir)

import sysmanager_pb2
import sysmanager_pb2_grpc

def test_unary(stub):
    print("\n--- Testing Unary RPC ---")
    
    # 1. GetSystemStatus
    req1 = sysmanager_pb2.StatusRequest(component_name="dock_core_python")
    reply1 = stub.GetSystemStatus(req1)
    print(f"Success! Online: {reply1.is_online} | State Enum: {reply1.current_state}")

    # 2. TriggerEnclosureControl
    req2 = sysmanager_pb2.EnclosureControlRequest(action=1)
    reply2 = stub.TriggerEnclosureControl(req2)
    print(f"Success! Enclosure Status code: {reply2.status}")


def test_server_streaming(stub):
    print("\n--- Testing Server Streaming (Weather Telemetry) ---")
    req = sysmanager_pb2.WeatherRequest(frequency_ms=200)
    
    # The server returns a stream, which Python treats as a standard iterable
    try:
        for telemetry in stub.StreamWeatherTelemetry(req):
            print(f"  [Stream Rx] Wind Speed: {telemetry.wind_speed:.1f} | Temp: {telemetry.temperature:.1f}")
    except grpc.RpcError as e:
        print(f"Stream ended or failed: {e}")
    print("Server Stream Finished.")


def generate_firmware_chunks():
    """Generator function to yield firmware chunks one by one."""
    for i in range(3):
        print(f"  [Stream Tx] Sending chunk {i}...")
        yield sysmanager_pb2.FirmwareChunk(
            chunk_index=i, 
            data_payload=f"binary_data_chunk_{i}"
        )
        time.sleep(0.1)

def test_client_streaming(stub):
    print("\n--- Testing Client Streaming (Firmware Upload) ---")
    
    # Pass the generator function directly to the stub
    # Python will automatically pull from the generator and stream it to the C++ server
    reply = stub.UploadFirmware(generate_firmware_chunks())
    print(f"Upload Success! Total Bytes processed: {reply.total_bytes_received}")


def generate_motor_commands():
    """Generator function for Bidirectional control loop."""
    for i in range(1, 4):
        target = 10.5 * i
        print(f"  [BiDi Tx] Sending Target Position: {target}")
        yield sysmanager_pb2.MotorCommand(target_position=target)
        time.sleep(0.25)

def test_bidirectional_streaming(stub):
    print("\n--- Testing Bidirectional Streaming (Motor Control) ---")
    
    # Pass the generator to the stub, and iterate over the returned response stream
    responses = stub.ActiveMotorControl(generate_motor_commands())
    
    for telemetry in responses:
        print(f"  [BiDi Rx] Telemetry - Pos: {telemetry.current_position:.1f} | Moving: {telemetry.is_moving}")
    print("Bidirectional Stream Finished.")


def run():
    target = 'unix:///tmp/sysmanager_grpc.sock'
    
    print(f"Attempting to connect to C++ Server at {target}...")
    
    # Create the insecure channel and instantiate the stub
    with grpc.insecure_channel(target) as channel:
        stub = sysmanager_pb2_grpc.SystemManagerStub(channel)
        
        print("========== gRPC Python Client Started ==========")
        test_unary(stub)
        test_server_streaming(stub)
        test_client_streaming(stub)
        test_bidirectional_streaming(stub)

if __name__ == '__main__':
    run()