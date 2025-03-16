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

unsigned int start_server()
{
    initWinsock();

    struct sockaddr_in server_addr;
    socklen_t server_addr_size = sizeof(server_addr);

    /*
     * add this because: you have to add a refernce
     * of the client variables in the server code.
     */

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    // socket creation

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
        return 1;

    // binding

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        return 2;

    // listen for a connection

    if (listen(server_socket, 1) < 0)
        return 3;
    
    std::cout << "server is waiting for connection..." << std::endl;

    // accept a connection 

    new_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_size);

    if (new_socket < 0)
        return 4;

    std::cout << "listening on port " << PORT << std::endl;

    return 0;
}