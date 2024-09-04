#include "Server.h"
#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void Server::bind_socket(int port) {
    if (!is_valid_port(port)) {
        throw CommsException("Invalid port number: " + std::to_string(port));
    }

    try {
        struct sockaddr_in address;
        int opt = 1;

        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw CommsException("setsockopt failed: " + std::string(strerror(errno)));
        }

        if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
            throw CommsException("Bind failed: " + std::string(strerror(errno)));
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

void Server::listen_for_connections() {
    if (listen(sock, 3) < 0) {
        throw CommsException("Listen failed: " + std::string(strerror(errno)));
    }
}

int Server::accept_connection() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        throw CommsException("Accept failed: " + std::string(strerror(errno)));
    }
    return new_socket;
}

void Server::handle_client(int client_socket) {
    try {
        char buffer[1024] = {0};

        while (true) {
            int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (valread <= 0) {
                if (valread == 0) {
                    std::cout << "Client disconnected" << std::endl;
                } else {
                    perror("recv failed");
                }
                break;
            }

            buffer[valread] = '\0';
            std::cout << "Client: " << buffer << std::endl;

            if (strcmp(buffer, "QUIT") == 0) break;

            if (send(client_socket, "Message Received", 16, 0) < 0) {
                perror("send failed");
                break;
            }
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
    }

    close(client_socket);
}

void Server::run(int port) {
    try {
        bind_socket(port);
        listen_for_connections();
        std::cout << "Server listening on port " << port << "..." << std::endl;

        while (true) {
            int client_socket = accept_connection();
            if (client_socket < 0) {
                std::cerr << "Failed to accept connection" << std::endl;
                continue;
            }
            std::thread client_thread([this, client_socket]() {
                this->handle_client(client_socket);
            });
            client_thread.detach();
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}
