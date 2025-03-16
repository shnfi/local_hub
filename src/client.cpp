#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 54321

int client_socket;
struct sockaddr_in client_addr;
socklen_t client_addr_size = sizeof(client_addr);

unsigned int start_client(std::string ip)
{
    // socket creation 

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
        return 1;

    // specifying the destination 

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &client_addr.sin_addr) <= 0)
        return 1;

    // make a connection 

    while (connect(client_socket, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
        std::cout << "[x] error while trying to make the connection! trying again..." << std::endl;
        sleep(1);
    }

    std::cout << "connected to " << ip << std::endl;

    return 0;
}
