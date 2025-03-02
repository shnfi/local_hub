#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 12345

int client_socket;
struct sockaddr_in client_addr;
socklen_t client_addr_size = sizeof(client_addr);

void start_client(std::string ip)
{

    // socket creation 

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
    {
        std::cout << "[x] error while starting server!" << std::endl;
        exit(1);
    }

    // specifying the destination 

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip.c_str(), &client_addr.sin_addr) <= 0)
    {
        std::cout << "[x] error while processing the address!" << std::endl;
        exit(1);
    }

    // make a connection 

    if (connect(client_socket, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
    {
        std::cout << "[x] error while trying to make the connection!" << std::endl;
        exit(1);
    }

    std::cout << "connected to " << ip << std::endl;
}