#include "Server.h"
#include "Comms.h"

#include <iostream>
#include <thread>
#include <cstring>
#include <unordered_map>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Map to store client IDs and associated socket descriptors
std::unordered_map<int, std::string> client_ids;
int client_counter = 0;  // Counter to generate unique client IDs
int last_client_socket = -1;  // Keep track of the last client that sent a message

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

        // Set socket options to allow reuse of address
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            throw CommsException("setsockopt failed: " + std::string(strerror(errno)));
        }

        // Bind the socket to the port
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
    if (listen(sock, 5) < 0) {  // Listen for connections with a backlog of 5
        throw CommsException("Listen failed: " + std::string(strerror(errno)));
    }
    DEBUG_PRINT("Server listening for connections.");
}

int Server::accept_connection() {
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);
    int client_socket = accept(sock, (struct sockaddr *)&client_address, &addrlen);

    if (client_socket < 0) {
        throw CommsException("Accept failed: " + std::string(strerror(errno)));
    }

    // Convert client IP to a readable format and retrieve the port number
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_address.sin_port);

    // Generate a unique ID for the client and store it
    std::string client_id = "client_" + std::to_string(++client_counter);
    client_ids[client_socket] = client_id;

    DEBUG_PRINT("Accepted client from IP: " + std::string(client_ip) + " on port: " + std::to_string(client_port));
    DEBUG_PRINT("Assigned ID: " + client_id);

    return client_socket;
}

// Handle communication with the client
void Server::handle_client(int client_socket) {
    std::string client_id = client_ids[client_socket];
    last_client_socket = client_socket;  // Keep track of the last active client

    char buffer[1024] = {0};  // Buffer to store messages
    while (true) {
        int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (valread <= 0) {
            if (valread == 0) {
                DEBUG_PRINT(client_id + " disconnected.");
            } else {
                std::cerr << "recv failed for " << client_id << ": " << strerror(errno) << std::endl;
            }
            break;
        }

        buffer[valread] = '\0';  // Null-terminate the received message

        // Clear the current input line before printing the received message
        std::cout << "\r\033[K";  // Clears the current line in the terminal
        std::cout << client_id << ": " << buffer << std::endl;  // Print the received message

        // Re-display the input prompt after printing the message
        std::cout << "Enter message to send (or specify client_id:message): " << std::flush;

        if (strcmp(buffer, "QUIT") == 0) break;  // Handle "QUIT" command
    }

    close(client_socket);
    client_ids.erase(client_socket);  // Remove client ID on disconnect
    DEBUG_PRINT("Client socket closed for " + client_id);
}



// Server main loop for managing connections and sending messages
void Server::run(int port) {
    bind_socket(port);
    listen_for_connections();
    std::cout << "Server listening on port " << port << std::endl;

    std::thread([this]() {
        // Thread for reading input from the server's terminal
        while (true) {
            std::string message;
            std::cout << "Enter message to send (or specify client_id:message): ";
            std::getline(std::cin, message);

            if (message.find(':') != std::string::npos) {
                // If input is in the format client_id:message, send to the specified client
                auto delimiter_pos = message.find(':');
                std::string client_id = message.substr(0, delimiter_pos);
                std::string client_message = message.substr(delimiter_pos + 1);

                int client_socket = -1;
                for (const auto& pair : client_ids) {
                    if (pair.second == client_id) {
                        client_socket = pair.first;
                        break;
                    }
                }

                if (client_socket != -1) {
                    if (send(client_socket, client_message.c_str(), client_message.length(), 0) < 0) {
                        std::cerr << "Failed to send message to client " << client_id << std::endl;
                    }
                } else {
                    std::cout << "Client " << client_id << " not found." << std::endl;
                }
            } else {
                // Otherwise, send the message to the last active client
                if (last_client_socket != -1) {
                    if (send(last_client_socket, message.c_str(), message.length(), 0) < 0) {
                        std::cerr << "Failed to send message to last active client." << std::endl;
                    }
                } else {
                    std::cout << "No active clients to send the message to." << std::endl;
                }
            }
        }
    }).detach();

    while (true) {
        int client_socket = accept_connection();  // Accept new clients
        std::thread client_thread([this, client_socket]() {
            this->handle_client(client_socket);
        });
        client_thread.detach();
    }
}
