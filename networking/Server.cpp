#include "Server.h"
#include <iostream>
#include <thread>
#include <cerrno> 
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void Server::bind_socket()
{
    struct sockaddr_in address;
    int opt = 1;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }
}

void Server::listen_for_connections()
{
    if (listen(sock, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }
}

int Server::accept_connection()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

void Server::handle_client(int client_socket)
{
    char buffer[1024] = {0};

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (valread <= 0)
        {
            if (valread == 0)
            {
                std::cout << "Client disconnected" << std::endl;
            }
            else
            {
                std::cerr << "Failed to read from socket" << std::endl;
            }
            break;
        }
        buffer[valread] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        if (strcmp(buffer, "QUIT") == 0)
            break;

        std::string response = "Message Received";
        if (send(client_socket, response.c_str(), response.length(), 0) < 0)
        {
            std::cerr << "Failed to send message" << std::endl;
            break;
        }
    }

    close(client_socket);
}

void Server::run()
{
    bind_socket();
    listen_for_connections();
    std::cout << "Server listening on port 8080..." << std::endl;

    while (true)
    {
        int client_socket = accept_connection();
        std::thread client_thread([this, client_socket]() {
            this->handle_client(client_socket);
        });
        client_thread.detach();
    }

    close(sock);
}
