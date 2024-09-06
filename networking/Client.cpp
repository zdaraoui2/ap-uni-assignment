#include "Client.h"
#include "Comms.h"

#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

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
    // Create a thread to handle receiving messages from the server
    std::thread receive_thread([this]() {
        try {
            char buffer[1024] = {0};
            while (true) {
                // Receive messages from the server
                int valread = receive_message(buffer, sizeof(buffer));

                if (valread > 0) {
                    // Move cursor to the beginning of the line, print the message, and then reprint the prompt
                    std::cout << "\rServer: " << buffer << std::endl;
                    DEBUG_PRINT("Received message from server: " + std::string(buffer));
                    // Reprint the input prompt for the user
                    std::cout << "Enter message: ";
                    std::cout.flush();
                } else if (valread == 0) {
                    // If the server has closed the connection
                    std::cout << "Server closed the connection." << std::endl;
                    DEBUG_PRINT("Server closed connection, client is exiting.");
                    break;
                } else {
                    // Handle receive error
                    std::cerr << "Receive message failed: " << strerror(errno) << std::endl;
                    DEBUG_PRINT("Receive error.");
                    break;
                }
            }
        } catch (const CommsException& e) {
            std::cerr << e.what() << std::endl;
        }
    });

    // Main thread handles user input
    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);  // Get user input

        send_message(message);  // Send the message to the server
        DEBUG_PRINT("Message sent: " + message);

        // If user types "QUIT", exit the loop
        if (message == "QUIT") {
            DEBUG_PRINT("Client is exiting...");
            break;
        }
    }

    // Join the receive thread to make sure it finishes properly
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    
    close_socket();  // Close the socket after everything is done
    DEBUG_PRINT("Socket closed.");
}
