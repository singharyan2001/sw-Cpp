# GUIDE 02 - Concurrency Workflow with threads in gRPC

In your previous sequential examples, the client waited for one operation to finish entirely before starting the next. In a real system, this is unacceptable. You cannot wait for a 10-second firmware upload to finish before checking if an emergency stop was triggered.

To achieve real-time constraints, we move to a Concurrent Architecture. We achieve this by combining gRPC's native HTTP/2 Multiplexing with C++ Multi-threading.

**Note:** In the next guide we will explore asynchronous based workflow with gRPC in C++

## The Thread-Safety Rules of gRPC
Before writing multi-threaded gRPC code, we must memorize these three rules:
1. `grpc::channel` is **Thread-Safe**: Multiple threads can share the exact same channel simultaneously. You only ever need ONE channel pointing to `/tmp/sysmanager_grpc.sock`. It natively multiplexes all your data.
2. `grpc::stub` is **Thread-Safe**: Multiple threads can call RPC methods on the exact same Stub object simultaneously without any C++ mutexes.
3. **Stream Objects (Readers/Writers) are NOT Thread-Safe:** You cannot have two threads calling `Write()` on the same `ClientReaderWriter` stream object at the same time. One thread must own the stream, or you must use a `std::mutex` to protect it.

## The Multi-Threaded Client Architecture
For the `dock-core` client, the optimal architecture is the "Dedicated Thread Pattern".
1. **Main Thread (Command & Control):** Stays awake, handles system logic, and fires Unary RPCs (like Enclosure actions) on demand.
2. **Telemetry Thread:** A background `std::thread` dedicated entirely to sitting in a `while(reader->Read())` loop, pulling weather data off the socket.
3. **Motor Control Thread:** A background `std::thread` dedicated to the Bidirectional stream.

## Handling Graceful Shutdowns (`TryCancel`)
If a background thread is permanently blocked waiting for weather data via `reader->Read()`, how do you stop it? You use the `grpc::ClientContext`. If the main thread calls `context->TryCancel()`, gRPC instantly breaks the stream connection. The `Read()` function will immediately return false, allowing the background thread to exit cleanly.

## Code Example: The Concurrent Client


## What about the Server Side?
You do not need to write custom C++ thread-pooling for the gRPC Server to support this!

When you use the generated synchronous `SystemManager::Service` (exactly like we did in `server.cpp`), gRPC automatically creates an elastic thread pool on the server.
- When the client connects to `StreamWeatherTelemetry`, the Server assigns one thread from its pool to loop and write data to the client.
- When the client suddenly fires `TriggerEnclosureControl` in the middle of the telemetry stream, the Server instantly wakes up another thread from its pool to handle that Unary call concurrently.

This means your Unix Domain Socket is now achieving true full-duplex multiplexing without you having to write a single complex C-level mutex or epoll call!
---