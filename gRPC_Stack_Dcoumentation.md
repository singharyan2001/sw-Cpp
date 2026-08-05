# gRPC Stack Documentation
The gRPC stack comprises of three components i.e. UDS or TCP/IP, gRPC, and Protobuf.

## Table of Contents
- [gRPC Stack Documentation](#grpc-stack-documentation)
  - [Table of Contents](#table-of-contents)
  - [Roadmap / Plan](#roadmap--plan)
  - [Recommended Learning Resources](#recommended-learning-resources)
  - [gRPC \& Protobuf Overview](#grpc--protobuf-overview)
    - [How does gRPC Work?](#how-does-grpc-work)
      - [Example](#example)
  - [The Asynchronous Nature of gRPC](#the-asynchronous-nature-of-grpc)
    - [Asynchronous servers](#asynchronous-servers)
    - [Client implementations](#client-implementations)
    - [Benefits of asynchronous approach](#benefits-of-asynchronous-approach)
  - [gRPC Architecture](#grpc-architecture)
    - [Key components of gRPC architecture](#key-components-of-grpc-architecture)
      - [gRPC Core](#grpc-core)
      - [Protocol Buffers](#protocol-buffers)
      - [Service definition](#service-definition)
      - [gRPC clients and servers](#grpc-clients-and-servers)
  - [gRPC Core Concepts](#grpc-core-concepts)
    - [Channel](#channel)
    - [Lifecycle](#lifecycle)
  - [Setting up](#setting-up)
    - [Linux](#linux)


## Roadmap / Plan
To master this stack, tackle the concepts in this specific order:
- **Phase 1: Protocol Buffers (The Data Definition):** Before you can send commands, you must define the data. You will learn a new syntax that is independent of C++.
    - The `.proto` Syntax: Learn how to define `message` blocks (like C structs).
    - Scalar Types: Understand Protobuf types (`int32`, `float`, `string`) and how they map to C++ types (`int32_t`, `float`, `std::string`).
    - The `protoc` Compiler: Learn how to manually run the Protobuf compiler in your terminal to generate the `.pb.cc` and `.pb.h` files from your `.proto` file.
- **Phase 2: gRPC Services (The Action Definition):** Once your data is defined, you define the actions (the Remote Procedure Calls).
    - Service Definitions: Learn how to add a `service` block inside your `.proto` file.
    - Unary RPCs: The basic "Call and Response" (e.g., Client requests sensor data, Server replies with a single message).
    - Streaming RPCs (Optional but powerful): Learn Server Streaming (Client sends one request, Server continuously sends data back) which is excellent for telemetry.
- **Phase 3: The Build System (The Hardest Part):** Do not try to compile gRPC manually with g++. It requires linking dozens of libraries.
    - CMake Integration: Learn how to use `find_package(gRPC)` and `find_package(Protobuf)`.
    - Custom Commands: Learn the CMake commands that automatically invoke protoc whenever you change your `.proto` file, so the C++ headers are generated seamlessly during the build process.
- **Phase 4: C++ Implementation (The Stubs):** This is where you write your actual firmware logic.
    - Server Implementation: Learn how to inherit from the auto-generated gRPC base class (e.g., `Dockstack::Service`) and override its virtual functions to implement your logic.
    - Client Implementation: Learn how to instantiate a Stub (a local proxy object) and call its methods.
- **Phase 5: Tying it to Unix Domain Sockets:** This is the final, easy step.
    - Learn the specific gRPC API calls to swap the default TCP transport for your UDS transport:
        - Server: `builder.AddListeningPort("unix:///tmp/dock.sock", grpc::InsecureServerCredentials());`
        - Client: `grpc::CreateChannel("unix:///tmp/dock.sock", grpc::InsecureChannelCredentials());`

## Recommended Learning Resources
- The Official Route (Start Here)
    - **grpc.io C++ Basics Tutorial:** This is the gold standard. It walks you through building a `RouteGuide` application. Do not skip this. It forces you to write the `.proto` file, configure CMake, and write the C++ client/server.
    - **How to use it:** Follow it exactly as written (which uses TCP). Once it works, change the connection strings to `unix:///tmp/routeguide.sock` to prove your UDS knowledge.
- Video Playlist (Youtube)
    - **CodeToCode — "gRPC C++ Tutorial":** This channel has a phenomenal, step-by-step playlist specifically for gRPC in C++. They dedicate entire videos to the CMake setup, writing the `.proto` file, and implementing Unary and Streaming RPCs.
    - **Hussein Nasser — "gRPC Crash Course":** While not purely C++, Hussein is a master at explaining backend engineering and networking. Watch his gRPC crash course to understand the why behind HTTP/2 framing, Protobuf binary encoding, and multiplexing before you write the code.
- Udemy Course (for reference/concept learning)
    - gRPC [Golang] Master Class: Build Modern API & Microservices
    - gRPC [Java] Master Class: Build Modern API & Micro services
    - gRPC Mastery: High-Performance Client-Server Communication
- Other Courses
    - https://www.educative.io/courses/introduction-to-grpc?utm_campaign=brand_educative&utm_source=google&utm_medium=ppc&utm_content=performance_max_india&utm_term=&aff=K3Zq&utm_term=&utm_campaign=%5BNew%5D+Performance+Max&utm_source=adwords&utm_medium=ppc&hsa_acc=5451446008&hsa_cam=18931439518&hsa_grp=&hsa_ad=&hsa_src=x&hsa_tgt=&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3&gad_source=1&gad_campaignid=18924941403&gbraid=0AAAAADfWLuTsPRcvTtYNlrh9gfKkW_IFX&gclid=CjwKCAjwgO7RBhBKEiwAZNP85nSeYncC0jw4jF-lK1LDJhyFAYJML3h7NR9vq46YGWYjXILXW5gDehoCgXQQAvD_BwE

- Youtube Videos for References
    - https://www.youtube.com/watch?v=UMwQjFzTQXw [ByteByteGo]      [HTTP/1 vs HTTP/2 vs HTTP/3]
    - https://www.youtube.com/watch?v=j9QmMEWmcfo [ByteByteGo]      [SSL, TLS, HTTP Explained]
    - https://www.youtube.com/watch?v=gnchfOojMk4 [ByteByteGo]      [gRPC & Protbuf Concept Explaination Video 1]
    - https://www.youtube.com/watch?v=hVrwuMnCtok [IBM-Technology]  [gRPC & Protbuf Concept Explaination Video 2]
    - https://grpc.io/docs/what-is-grpc/core-concepts/ [grpc.io]    [gRPC core concepts - 2024 keynote]

---

## gRPC & Protobuf Overview

In gRPC, communication between a client and a server happens over a network connection, typically using HTTP/2 as the underlying protocol. When a client sends a request to a server, the request is sent as an HTTP/2 message to the server, and the server responds with an HTTP/2 message containing the response. So gRPC inherits all the benefits of HTTP/2.

HTTP/2 provides multiple improvements features such as
1. **One TCP connection:** HTTP/2 uses one connection which is long lasting. This connection is shared by multiple requests and responses.
2. **Server Push:** HTTP/2 uses something called server push which means that it can push multiple messages for one request from the client. The client doesn't need to ask for data again and again, the server sends it automane our Unix Domain Socket path
    std::string server_address("unix:///tmp/sysmanager_grpc.sock");

    SystemManagerServiceImpl service;
    grpc::ServerBuilder builder;

    // Tell gRPC to listen on our UDS without any encryption
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register our service logic
    builder.RegisterService(&service);

    // Register our server thread
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "gRPC Server Listening on " << server_address << std::endl;

    // Block until the server shuts down
    server->Wait();
}tically when it is ready. This leads to a reduction in the number of requests.
3. **Multiplexing:** HTTP/2 uses multiplexing which means that the server and client can push multiple messages in parallel over the same TCP connection.
4. **Header Compression:** The headers and data both are compressed to binary format which leads to a reduction in payload. This is also why protocol buffers are a great match for HTTP/2.
5. **Security:** Most implementations (like modern browsers) only support HTTP/2 over TLS making the communication secure.

The gRPC framework takes care of marshaling the request and response messages to and from protobuf format, which is a more compact and efficient data format than alternatives like XML or JSON. gRPC supports four types of RPC methods: unary, server streaming, client streaming, and bidirectional streaming.

Protocol Buffers are a language-agnostic binary serialization format developed by Google. It is used to define the structure of the data being sent in gRPC messages. A `.proto` file defines the message types and RPC methods used in the service. Once the `.proto` file is defined, you can use the protocol buffer compiler to generate client and server code in a variety of programming languages, including Java, C++, Python, and Go.

One key aspect of gRPC’s operation is the use of protocol buffers for data serialization. Protocol buffers are a language and platform-neutral way of representing structured data. gRPC uses them to define the messages sent between client and server and the structure of the service itself. By using a single serialization format across all platforms and languages, gRPC enables easy interoperability between different systems.

### How does gRPC Work?
To use gRPC in a C++ application, we first need to define a `.proto` file that describes the service we want to create. The protoc compiler compiles the `.proto` file to generate C++ classes that we can use to create clients and servers. To understand how gRPC works, let’s walk through a typical flow from a gRPC client to a gRPC server.

When a gRPC client wants to make a remote procedure call to a gRPC server, it creates a stub. This stub acts as a client-side proxy for the remote service defined by the service contract. The client then calls methods on the stub, passing in any necessary arguments, and the stub handles the details of encoding the request, sending it over the network, and decoding the response.

On the server side, the gRPC runtime listens for incoming requests on a network port. When a request comes in, it decodes the request data and invokes the appropriate method on the server implementation. The server implementation processes the request and generates a response, which is then encoded and sent back to the client.

#### Example
Consider that we have a `Greeter` service that receives a request and sends a greeting as a response. A gRPC client invokes the `Greeter` Service on the gRPC server.

![Example](/doc-imgs/gRPC-Example.png)

1. When the client makes a gRPC call to the service, it invokes the client stub (code generated by gRPC at build time). The data passed into the stub is encoded into Protocol Buffers.
2. The encoded request data is sent to the transport layer.
3. gRPC sends the data over the network as a stream of HTTP/2 data frames.
4. The `Greeter` service receives the packets from the network and sends them for decoding.
5. The binary format is converted into C++ (which is the language in which the server is implemented), and the request is processed by the server.
6. The result returned from the server application gets encoded into Protocol Buffers.
7. The encoded response data is sent to the transport layer.
8. gRPC sends the data back to the client as a stream of HTTP/2 data frames.
9. The client receives the packets from the network and sends them to the client stub for decoding.
10. The client application receives the decoded response.

Note: The Network here, can be replaced with Unix DOmain Sockets if being implemented for internal machine communication.

## The Asynchronous Nature of gRPC
gRPC operates on an asynchronous model, leveraging this appne our Unix Domain Socket path
    std::string server_address("unix:///tmp/sysmanager_grpc.sock");

    SystemManagerServiceImpl service;
    grpc::ServerBuilder builder;

    // Tell gRPC to listen on our UDS without any encryption
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register our service logic
    builder.RegisterService(&service);

    // Register our server thread
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "gRPC Server Listening on " << server_address << std::endl;

    // Block until the server shuts down
    server->Wait();
}roach for efficient communication between clients and servers. Understanding this asynchronous nature is crucial in comprehending how gRPC excels in handling concurrent requests without blocking the main thread.

### Asynchronous servers
In gRPC, the server operates asynchronously, meaning it doesn’t block while waiting for requests. Instead, it uses non-blocking I/O operations, allowing it to handle multiple requests simultaneously. This asynchronous nature is fundamental; it enables the server to handle a large number of incoming requests concurrently, without waiting for each request to complete before processing the next one.

By employing non-blocking operations, the server can juggle multiple tasks concurrently. The system doesn't get bogged down by waiting for each request to finish. When a request comes in, the server can initiate the necessary processing and continue to handle other requests concurrently. This approach maximizes resource utilization and minimizes latency, as the server doesn’t have to wait for each request to finish before moving on to the next. It enhances the server's throughput and responsiveness. Asynchronous processing plays a pivotal role in gRPC's scalability. By avoiding blocking operations, the server can efficiently handle an increasing number of concurrent requests. This scalability is crucial in distributed systems, allowing gRPC to serve a larger number of clients without sacrificing performance.

### Client implementations
On the client side, gRPC offers flexibility in choosing between synchronous and asynchronous communication patterns based on the application’s requirements.
1. **Synchronous communication:** In synchronous communication, the client sends a request and waits for the corresponding response before proceeding with further operations. This method ensures a deterministic flow where the client relies on immediate responses. Consider a banking application where a client requests a balance inquiry. The client sends the request and awaits the immediate response, crucial for displaying real-time account information.
2. **Asynchronous communication:** Contrarily, asynchronous communication allows the client to send requests while continuing other operations until the responses arrive. It can later check for responses or be notified asynchronously when responses arrive. This mode is ideal for scenarios where immediate responses are not critical or when managing multiple ongoing requests concurrently is necessary. In a data-intensive scenario, such as fetching multiple records from a database, the client can make asynchronous requests for each record and continue other operations while periodically checking for responses. This approach allows the client to optimize its workflow and manage multiple ongoing tasks concurrently.

### Benefits of asynchronous approach
- **Scalability:** Asynchronous handling allows the server to efficiently manage numerous requests simultaneously, enhancing scalability.
- **Performance:** Non-blocking operations prevent the server from getting tied up with individual requests, thereby improving overall system performance.
- **Resource utilization:** It optimizes resource utilization by allowing the server to process multiple requests without unnecessary waiting.

The asynchronous nature of gRPC, with its non-blocking server operations, empowers the framework to handle concurrent requests efficiently. It offers the flexibility for clients to choose between synchronous and asynchronous communication patterns based on the application’s requirements, ensuring optimal performance, scalability, and resource utilization.

## gRPC Architecture

### Key components of gRPC architecture
The architecture of gRPC is built on a modular design, with distinct components working together to enable efficient communication between distributed systems. The key components are:
1. **Service definition:** Defines remote methods, their input/output types, and metadata using protocol buffers.
2. **Protocol buffers:** A compact, language-neutral serialization format for efficient data exchange.
3. **gRPC core**: Handles network communication, message encoding/decoding, and concurrency using HTTP/2.
4. **gRPC clients and servers:** Implement business logic, process requests, and communicate via the service definition.

#### gRPC Core
gRPC Core handles the underlying communication protocols, primarily based on HTTP/2. HTTP/2 provides features like multiplexing, flow control, and header compression, which contribute to more efficient communication between clients and servers. The library supports Protocol Buffers (protobuf) as the default serialization format for structured data. gRPC Core is responsible for encoding and decoding messages in the binary format defined by Protocol Buffers. The library handles concurrency and threading aspects of gRPC communication. It manages the parallel processing of multiple requests and responses efficiently.

#### Protocol Buffers
Protocol buffers are designed to be efficient, compact, and extensible, making them well-suited for communication between distributed systems. Key features of Protocol Buffers include:
1. **Schema definition language:** Protocol Buffers use a simple and human-readable interface definition language (IDL) to define the structure of the data. The schema defines the types of messages that can be exchanged between gRPC clients and servers.
2. **Compact binary format:** Protocol Buffers encode data in a binary format that is more compact than traditional JSON or XML. This leads to smaller message sizes and more efficient data transmission over the network.
3. **Efficiency:** Protocol Buffers' serialization and deserialization processes are highly efficient, making them suitable for high-performance applications. The encoding and decoding operations are faster than those of text-based formats.
4. **Versioning and backward compatibility:** Protocol Buffers support backward compatibility, allowing new fields to be added to messages without breaking existing clients. This makes it easier to evolve data structures over time without disrupting communication between services.
5. **Code generation:** gRPC uses Protocol Buffers to define service methods and message formats. Based on these definitions, gRPC generates client and server code in various programming languages. This generated code provides type-safe access to the defined services and message structures, reducing the likelihood of runtime errors.
6. **Extensibility:** Protocol Buffers provide a mechanism for extending message definitions without breaking existing clients. This extensibility is crucial for evolving data models in a distributed system.

#### Service definition
In gRPC architecture, the Service Definition plays a central role in specifying the methods and message types that are part of a gRPC service. It is defined using Protocol Buffers. The methods in the proto file define the operations that clients can invoke on the server. Each method is associated with a request message and a response message.

The Service Definition includes the definition of message types used as parameters and return values for the service methods. These message types are also defined using Protocol Buffers and include fields with specific data types. The Service Definition may include syntax and options to define additional characteristics of the service. For example, it may specify whether a service uses unary or streaming methods, and it can include other configuration options.

#### gRPC clients and servers
In gRPC, the client and server implementations use the gRPC Core library along with the Service Definition to facilitate communication between them. Let’s explore how the client and server are implemented:


Server Implementation: The implementation involves the following steps:
1. Define service methods: The server defines methods corresponding to the service methods specified in the Service Definition. These methods handle incoming requests from clients.
2. Implement service logic: For each service method, the server implements the actual logic to process the incoming requests and generate the appropriate responses.
3. Bind service implementation to gRPC server: The server binds the implemented service to a gRPC server instance using gRPC Core. This involves creating a gRPC server, adding the service implementation to it, and specifying the network address to listen on.
4. Start gRPC server: The server starts the gRPC server, making it ready to accept incoming requests from clients.

Client Implementation: The implementation involves the following steps:
1. Create gRPC channel: The client creates a gRPC channel to connect to the server. The channel encapsulates the connection details and provides a means for the client to communicate with the server.
2. Generate stub: The client generates a stub based on the Service Definition. The stub provides a convenient and type-safe API for invoking remote service methods.
3. Invoke service methods: The client uses the generated stub to invoke the service methods defined in the Service Definition. It prepares request messages, sends them to the server, and receives the corresponding responses.

gRPC’s architecture is designed to be modular and extensible, allowing developers to customize and extend the functionality of the framework as needed. By separating the low-level network communication from the higher-level application logic, gRPC is able to provide a high-performance, scalable, and flexible framework for building distributed systems.

## gRPC Core Concepts

### Channel
- Channel is an abstract of the endpoint you can send or receive messages.
- Analogy: If Channel is like a waterpipe placeholder, then sub-channels are the real connections to backend services.
- gRPC during its lifecycle would create those sub-channels, dynamically select sub-channels to multiply rpcs over the channel and it would report channel status and finally tear down the sub-channels to return resources.

### Lifecycle
- gRPC is very simple to use, the application only needs to send a request of the stub that is created from the channel and stub is at the protobuf generated layer which is also first layer that you would see when you are using gRPC.
- The Stub creates a call towards the gRPC Runtime which then further creates a stream on the transport.
- In gRPC, an RPC which is a core or stream are fundamentally the same concept, just referred to by different names at various stages in their lifetime
- Lifecycle: Application (Channel), Stub (Protobuf plugin), gRPC Runtime (API Core), Transport.
- The Lifecycle is followed by both the Server and the Clients.
- gRPC supports 4 primitives i.e. Unary, Client Streaming, Server Streaming, and Bi-Directional Streaming.
- gRPC is built to be asynchronous, but does have synchronous blocking apis for special use case.

- Channel at the server layer, interceptors are useful tools to add a tasks that are independent of the methods but apply to all or most of the RPCs.
- Interceptors are very powerful middleware tools that add a task to modify or replace your cost, before and after they reach your destination at both the client and server side.
- Interceptors provides a clean way to address cutting edge concerns like logging, authentication, authorization, error handling, and monitoring etc without cluttering your main application logic.
- You can provide multiple server interceptors and thir orders matter, for example you are installing two client interceptors for caching and logging, you must place the logging intercept first and then caching interceptor, because if flipped, that would mean you are focusing more on communication and during an operation if a crash occurs, it would not reach the logging interceptor first and you may loose the crash being logged.

- Deadline/Timeout [11:07]


## Setting up

### Linux

Install all dependencies:
```bash
sudo apt install cmake build-essential autoconf libtool pkg-config libgrpc++-dev protobuf-compiler-grpc
```
