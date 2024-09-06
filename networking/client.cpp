#include "Client.h"
#include "Comms.h"

#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void Client::connect_to_server(const std::string& ip_address, int port) {
    // Check if the IP address and port number are valid before trying to connect
    if (!is_valid_ip(ip_address)) {
        throw CommsException("Invalid IP address format: " + ip_address);
    }

    if (!is_valid_port(port)) {
        throw CommsException("Invalid port number: " + std::to_string(port));
    }

    try {
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));  // Clear out the structure

        serv_addr.sin_family = AF_INET;  // Use IPv4
        serv_addr.sin_port = htons(port);  // Set the server's port number

        // Convert IP address from text to binary form
        if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
            throw CommsException("Invalid address or Address not supported");
        }

        create_socket();  // Create the client's socket
        DEBUG_PRINT("Socket created.");

        // Set a 5-second timeout for sending and receiving data
        struct timeval timeout;
        timeout.tv_sec = 5;  
        timeout.tv_usec = 0; 

        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw CommsException("Failed to set receive timeout: " + std::string(strerror(errno)));
        }

        if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw CommsException("Failed to set send timeout: " + std::string(strerror(errno)));
        }

        DEBUG_PRINT("Timeouts set");

        // Enable TCP keep-alive to check if the server is still there
        int enable_keepalive = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &enable_keepalive, sizeof(enable_keepalive)) < 0) {
            throw CommsException("Failed to enable TCP keep-alive: " + std::string(strerror(errno)));
        }

        DEBUG_PRINT("TCP keep-alive enabled.");

        // Attempt to connect to the server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            throw CommsException("Connection failed: " + std::string(strerror(errno)));
        }

        DEBUG_PRINT("Connected to server at " + ip_address + " on port " + std::to_string(port));
    } catch (const CommsException& e) {
        // Handle any exceptions, close the socket, and exit
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

void Client::run() {
    try {
        std::string message;
        while (true) {
            DEBUG_PRINT("Waiting for user input...");
            std::cout << "Enter message: ";
            std::getline(std::cin, message);  // Read user input

            send_message(message);  // Send the message to the server
            DEBUG_PRINT("Message sent: " + message);

            if (message == "QUIT") {
                DEBUG_PRINT("Client is exiting...");
                break;  // Exit the loop if the user wants to quit
            }

            char buffer[1024] = {0};  // Buffer to hold the server's response
            int valread = receive_message(buffer, sizeof(buffer));  // Receive the server's message

            if (valread > 0) {
                std::cout << "Server: " << buffer << std::endl;
                DEBUG_PRINT("Received message from server: " + std::string(buffer));
            } else if (valread == 0) {
                std::cout << "Server closed the connection." << std::endl;
                DEBUG_PRINT("Server closed connection, client is exiting.");
                break;  // Exit if the server closes the connection
            } else {
                std::cerr << "Failed to receive message from server. Exiting..." << std::endl;
                DEBUG_PRINT("Receive error.");
                break;  // Exit on receive error
            }
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
    }

    close_socket();  // Close the socket when done
    DEBUG_PRINT("Socket closed.");
}
