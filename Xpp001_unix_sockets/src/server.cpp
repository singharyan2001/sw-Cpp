// Unix Sockets in C++ : Server Side
#include <iostream>
#include <sys/socket.h>     // Core Socket functions
#include <sys/un.h>         // Unix Domain Socket structures (sockaddr_un)
#include <unistd.h>         // close(), read(), write()

/* Server Side
 * socker(), bind(), listen(), accept(), read() / write(), close()
*/

int main(){
    std::cout << "========== UNIX DOMAIN SOCKETS [SERVER SIDE] ==========\n";
    std::cin.get();
}