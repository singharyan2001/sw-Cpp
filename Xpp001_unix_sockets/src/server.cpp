// Unix Sockets in C++ : Server Side
#include <iostream>
#include <sys/socket.h>     // Core Socket functions
#include <sys/un.h>         // Unix Domain Socket structures (sockaddr_un)
#include <unistd.h>         // close(), read(), write()

#include <string>
#include <vector>
#include <cerrno>

#include <cstdint>

/* Server Side
 * socker(), bind(), listen(), accept(), read() / write(), close()
*/

/* RAII Wrapper to ensure file descriptors are always closed */
class ScopedFd {
private:
    int m_fd;
public:
    // CONSTRUCTOR
    explicit ScopedFd(int fd) : m_fd(fd) {
        std::cout << "[CONSTRUCTOR] Scoped File Descriptor Initialized\n";
    }
    // DESTRUCTOR
    ~ScopedFd(){
        if(m_fd >= 0) {
            close(m_fd);
            std::cout << "[DESTRUCTOR] Scoped File Descriptor Closed\n";
        }
    }
    // Internal APIs
    int get() const{
        return m_fd;
    }

    // Deleting copy constructor & copy assignment operator to preven double-closing the same FD
    ScopedFd(const ScopedFd& other) = delete;
    ScopedFd& operator=(const ScopedFd& other) = delete;
};

int main(){
    std::cout << "========== UNIX DOMAIN SOCKETS [SERVER SIDE] ==========\n";
    const char* socket_path = "/tmp/dock_core.sock";

    // 1. Clean up any leftover socket file from a previous crash
    unlink(socket_path);

    // 2. Create the socket
    ScopedFd server_socket(socket(AF_UNIX, SOCK_STREAM, 0));
    if(server_socket.get() == -1){
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return 1;
    }

    // 3. Prepare the address struct
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    // Note on memory safety: by using sizeof(addr.sun_path) - 1, we ensure that even if the socket path
    // is abnormally long, the character array reamins safely null-terminated.

    // 4. Bind the socket to the file path
    if(bind(server_socket.get(), (struct sockaddr*)&addr, sizeof(addr) ) == -1) {
        std::cerr << "Bind Failed: " << strerror(errno) << std::endl;
        return 1;
    }
    else{
        std::cout << "Bind process Success\n";
    }

    // 5. Listen for incoming connections (queue size of 5)
    if(listen(server_socket.get(), 5) == -1){
        std::cerr << "Listen Failed: " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "Server listening on " << socket_path << "..." << std::endl;

    // 6. Accept a connection (This blocks until a client connects)
    ScopedFd client_socket(accept(server_socket.get(), nullptr, nullptr));
    if(client_socket.get() == -1){
        std::cerr << "Accept Failed: " << strerror(errno) << std::endl;
        return 1;
    }
    else{
        std::cout << "Client Connection request detected! ... accepting request ... Connected to client!\n";
    }

    // 7. Read data using a C++ vector instead of raw char array
    std::vector<uint8_t> buffer(1024, 0);
    ssize_t bytes_read = read(client_socket.get(), buffer.data(), buffer.size() - 1);

    if(bytes_read > 0){
        std::cout << "Received: " << reinterpret_cast<char*>(buffer.data()) << std::endl;

        std::cout << "Sending back response to client!" << std::endl;

        // 8. Send a Response
        std::string response = "PONG from Server";
        write(client_socket.get(), response.c_str(), response.length());
    }

    // Unlink the file path to clean up the filesystem
    unlink(socket_path);
    std::cout << "Server shutting down cleanly." << std::endl;

    // std::cin.get();
    return 0;
}