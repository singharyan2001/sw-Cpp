// Unix Sockets in C++ : Client Side
#include <iostream>
#include <sys/socket.h>     // Core Socket functions
#include <sys/un.h>         // Unix Domain Socket structures (sockaddr_un)
#include <unistd.h>         // close(), read(), write()

/* Client Side
 * socket(), connect(), write() / read(), close
*/

int main(){
    std::cout << "========== UNIX DOMAIN SOCKETS [CLIENT SIDE] ==========\n";
    std::cin.get();
}