#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.cpp"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void initWinsock()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
}
#else
void initWinsock() {} // Empty for Linux/macOS
#endif

int server_socket, new_socket;

void start_server()
{
    initWinsock();

    struct sockaddr_in server_addr;
    socklen_t server_addr_size = sizeof(server_addr);

    struct sockaddr_in client_addr;  // ADD THIS!
    socklen_t client_addr_size = sizeof(client_addr);  // ADD THIS!

    // socket creation

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        std::cout << "[x] error while starting server!" << std::endl;
        exit(1);
    }

    // binding

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "[x] error while binding the server!" << std::endl;
        exit(1);
    }

    // listen for a connection

    if (listen(server_socket, 1) < 0)
    {
        std::cout << "[x] error while listening on the specified port" << std::endl;
        exit(1);
    }
    
    std::cout << "server is waiting for connection..." << std::endl;

    // accept a connection 

    new_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_size);

    if (new_socket < 0)
    {
        std::cout << "[x] error while accepting the connection!" << std::endl;
        exit(1);
    }
}