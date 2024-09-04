#include "Client.h"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void Client::connect_to_server(const std::string& ip_address, int port) {
    try {
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
            throw CommsException("Invalid address or Address not supported");
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            throw CommsException("Connection failed: " + std::string(strerror(errno)));
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
        close_socket();
        exit(EXIT_FAILURE);
    }
}

void Client::run() {
    try {
        std::string message;
        while (true) {
            std::cout << "Enter message: ";
            std::getline(std::cin, message);

            send_message(message);

            if (message == "QUIT")
                break;

            char buffer[1024] = {0};
            int valread = receive_message(buffer, sizeof(buffer));
            if (valread > 0) {
                std::cout << "Server: " << buffer << std::endl;
            }
            else if (valread == 0) {
                std::cout << "Server closed the connection." << std::endl;
                break;
            }
            else {
                std::cerr << "Failed to receive message from server. Exiting..." << std::endl;
                break;
            }
        }
    } catch (const CommsException& e) {
        std::cerr << e.what() << std::endl;
    }

    close_socket();
}
