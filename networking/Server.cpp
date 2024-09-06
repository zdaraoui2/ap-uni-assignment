#include "Server.h"
#include "Comms.h"

#include <iostream>
#include <thread>
#include <cstring>
#include <unordered_map>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>

std::unordered_map<int, std::string> client_ids;  // Map of socket to client ID
int client_counter = 0;  // Counter to generate unique client IDs

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

        DEBUG_PRINT("Socket bound to port " + std::to_string(port));
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

void Server::listen_for_connections() {
    if (listen(sock, 5) < 0) {  // 5 is the backlog size
        throw CommsException("listen_for_connections: Listen failed: " + std::string(strerror(errno)));
    }
    DEBUG_PRINT("Server listening for connections.");
}

int Server::accept_connection() {
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);
    int client_socket = accept(sock, (struct sockaddr *)&client_address, &addrlen);

    if (client_socket < 0) {
        throw CommsException("accept_connection: Accept failed: " + std::string(strerror(errno)));
    }

    // Convert the client's IP to a readable format and get the port number
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_address.sin_port);

    // Generate a unique ID for the client and store it with their socket
    std::string client_id = "client_" + std::to_string(++client_counter);
    client_ids[client_socket] = client_id;

    // Debug print statements for client info and ID assignment
    DEBUG_PRINT("Accepted client from IP: " + std::string(client_ip) + " on port: " + std::to_string(client_port));
    DEBUG_PRINT("Assigned ID: " + client_id);

    return client_socket;
}

void Server::handle_client(int client_socket) {
    std::string client_id = client_ids[client_socket];  // Retrieve the client's unique ID

    try {
        // Enable TCP keep-alive for this client
        int enable_keepalive = 1;

        if (setsockopt(client_socket, SOL_SOCKET, SO_KEEPALIVE, &enable_keepalive, sizeof(enable_keepalive)) < 0) {
            throw CommsException("handle_client: Failed to enable TCP keep-alive: " + std::string(strerror(errno)));
        }

        DEBUG_PRINT("TCP keep-alive enabled for client " + client_id);

        char buffer[1024] = {0};
        while (true) {
            // Receive data from the client
            int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (valread <= 0) {
                if (valread == 0) {
                    std::cout << "Client " + client_id + " disconnected" << std::endl;
                } else {
                    std::cerr << "handle_client: recv failed for " << client_id << ": " << strerror(errno) << std::endl;
                }
                break;
            }

            buffer[valread] = '\0';  // Null-terminate the received message
            std::cout << client_id << ": " << buffer << std::endl;
            DEBUG_PRINT("Received message from " + client_id + ": " + std::string(buffer));

            // Check if the client sent "QUIT" to end the connection
            if (strcmp(buffer, "QUIT") == 0) break;

            // Send acknowledgment back to the client
            if (send(client_socket, "Message Received", 16, 0) < 0) {
                std::cerr << "handle_client: send failed for " << client_id << ": " << strerror(errno) << std::endl;
                DEBUG_PRINT("Failed to send message to client " + client_id);
                break;
            }

            DEBUG_PRINT("Sent acknowledgment to " + client_id);
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
    }

    // Close the client's socket and remove them from the client list
    close(client_socket);
    client_ids.erase(client_socket);  // Remove client ID from the map after disconnecting
    DEBUG_PRINT("Client socket closed for " + client_id);
}

void Server::run(int port) {
    try {
        bind_socket(port);  // Bind the server to the specified port
        listen_for_connections();  // Start listening for incoming connections
        std::cout << "Server listening on port " << port << std::endl;

        // Loop to accept and handle client connections
        while (true) {
            int client_socket = accept_connection();  // Accept a new client
            if (client_socket < 0) {
                std::cerr << "Failed to accept connection." << std::endl;
                DEBUG_PRINT("Failed to accept connection.");
                continue;
            }

            // Handle each client in a separate thread
            std::thread client_thread([this, client_socket]() {
                this->handle_client(client_socket);
            });
            client_thread.detach();  // Detach the thread to allow it to run independently
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}
