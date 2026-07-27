// Unix Sockets in C++ : Client Side
#include <iostream>
#include <sys/socket.h>     // Core Socket functions
#include <sys/un.h>         // Unix Domain Socket structures (sockaddr_un)
#include <unistd.h>         // close(), read(), write()

#include <string>
#include <vector>
#include <cerrno>

#include <cstdint>

/* Client Side
 * socket(), connect(), write() / read(), close
*/

// Reusing our RAII Wrapper
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
    std::cout << "========== UNIX DOMAIN SOCKETS [CLIENT SIDE] ==========\n";
    const char* socket_path = "/tmp/dock_core.sock";

    // 1. Create the socket
    ScopedFd client_socket(socket(AF_UNIX, SOCK_STREAM, 0));
    if(client_socket.get() == -1){
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. Prepare the address struct to match the server
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    // Note on memory safety: by using sizeof(addr.sun_path) - 1, we ensure that even if the socket path
    // is abnormally long, the character array reamins safely null-terminated.

    std::cout << "Connecting to server..." << std::endl;

    // 3. Connect to the server
    if(connect(client_socket.get(), (struct sockaddr*)&addr, sizeof(addr)) == 1){
        std::cerr << "Connection failed. Is the server running? Error: " << strerror(errno) << std::endl;
        return 1;
    }
    else{
        std::cout << "Connected to server!" << std::endl;
    }

    // 4. Send data
    std::string message = "PING from client";
    if(write(client_socket.get(), message.c_str(), message.length()) == -1){
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Sent: " << message << std::endl;

    // 5. Read the response
    std::vector<uint8_t> buffer(1024, 0);
    ssize_t bytes_read = read(client_socket.get(), buffer.data(), buffer.size() - 1);

    if(bytes_read > 0){
        std::cout << "Received: " << reinterpret_cast<char*>(buffer.data()) << std::endl;
    }

    std::cout << "Client shutting down cleanly." << std::endl;
    
    // std::cin.get();
    return 0;
}