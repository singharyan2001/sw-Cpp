package main

import (
	"context"
	"fmt"
	"io"
	"log"
	"time"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"

	// This import path assumes you initialize your Go module as "dock_core"
	// in the root of your project directory.
	pb "dock_core/build/generated/go"
)

func testUnary(client pb.SystemManagerClient) {
	fmt.Println("\n--- Testing Unary RPC ---")

	// 1. GetSystemStatus
	req1 := &pb.StatusRequest{ComponentName: "dock_core_go"}
	reply1, err := client.GetSystemStatus(context.Background(), req1)
	if err != nil {
		log.Fatalf("GetSystemStatus failed: %v", err)
	}
	fmt.Printf("Success! Online: %v | State Enum: %v\n", reply1.IsOnline, reply1.CurrentState)

	// 2. TriggerEnclosureControl
	req2 := &pb.EnclosureControlRequest{Action: 1}
	reply2, err := client.TriggerEnclosureControl(context.Background(), req2)
	if err != nil {
		log.Fatalf("TriggerEnclosureControl failed: %v", err)
	}
	fmt.Printf("Success! Enclosure Status code: %d\n", reply2.Status)
}

func testServerStreaming(client pb.SystemManagerClient) {
	fmt.Println("\n--- Testing Server Streaming (Weather Telemetry) ---")

	req := &pb.WeatherRequest{FrequencyMs: 200}
	stream, err := client.StreamWeatherTelemetry(context.Background(), req)
	if err != nil {
		log.Fatalf("Error opening stream: %v", err)
	}

	// Infinite loop to read from the stream until EOF (End Of File/Stream)
	for {
		telemetry, err := stream.Recv()
		if err == io.EOF {
			break // Server closed the stream cleanly
		}
		if err != nil {
			log.Fatalf("Stream read failed: %v", err)
		}
		fmt.Printf("  [Stream Rx] Wind Speed: %.1f | Temp: %.1f\n", telemetry.WindSpeed, telemetry.Temperature)
	}
	fmt.Println("Server Stream Finished.")
}

func testClientStreaming(client pb.SystemManagerClient) {
	fmt.Println("\n--- Testing Client Streaming (Firmware Upload) ---")

	stream, err := client.UploadFirmware(context.Background())
	if err != nil {
		log.Fatalf("Error opening client stream: %v", err)
	}

	for i := 0; i < 3; i++ {
		fmt.Printf("  [Stream Tx] Sending chunk %d...\n", i)
		chunk := &pb.FirmwareChunk{
			ChunkIndex:  int32(i),
			DataPayload: fmt.Sprintf("binary_data_chunk_%d", i),
		}

		if err := stream.Send(chunk); err != nil {
			log.Fatalf("Failed to send chunk: %v", err)
		}
		time.Sleep(100 * time.Millisecond)
	}

	// Tell the server we are done sending, and wait for the single response
	reply, err := stream.CloseAndRecv()
	if err != nil {
		log.Fatalf("Error receiving upload result: %v", err)
	}
	fmt.Printf("Upload Success! Total Bytes processed: %d\n", reply.TotalBytesReceived)
}

func testBidirectionalStreaming(client pb.SystemManagerClient) {
	fmt.Println("\n--- Testing Bidirectional Streaming (Motor Control) ---")

	stream, err := client.ActiveMotorControl(context.Background())
	if err != nil {
		log.Fatalf("Error opening bidi stream: %v", err)
	}

	// Ping-pong approach: Send a command, immediately wait for the reply
	for i := 1; i <= 3; i++ {
		target := float32(10.5 * float32(i))
		fmt.Printf("  [BiDi Tx] Sending Target Position: %.1f\n", target)

		req := &pb.MotorCommand{TargetPosition: target}
		if err := stream.Send(req); err != nil {
			log.Fatalf("Failed to send motor command: %v", err)
		}

		telemetry, err := stream.Recv()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatalf("Failed to receive telemetry: %v", err)
		}

		fmt.Printf("  [BiDi Rx] Telemetry - Pos: %.1f | Moving: %v\n", telemetry.CurrentPosition, telemetry.IsMoving)
		time.Sleep(250 * time.Millisecond)
	}

	// Cleanly close the sending direction of the stream
	stream.CloseSend()
	fmt.Println("Bidirectional Stream Finished.")
}

func main() {
	target := "unix:///tmp/sysmanager_grpc.sock"
	fmt.Printf("Attempting to connect to C++ Server at %s...\n", target)

	// Insecure because we are using a local Unix Domain Socket
	conn, err := grpc.NewClient(target, grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatalf("Failed to connect: %v", err)
	}
	defer conn.Close() // defer ensures this runs right before main() exits

	// Instantiate the generated client stub
	client := pb.NewSystemManagerClient(conn)

	fmt.Println("========== gRPC Go Client Started ==========")
	testUnary(client)
	testServerStreaming(client)
	testClientStreaming(client)
	testBidirectionalStreaming(client)
}
