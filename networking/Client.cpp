#include "Client.h"
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void Client::connect_to_server() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

}

void Client::run() {
    std::string message;
    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        if (send_message(message) < 0) {
            std::cerr << "Failed to send message" << std::endl;
            break;
        }

        if (message == "QUIT")
            break;

        char buffer[1024] = {0};
        int valread = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            std::cout << "Server: " << buffer << std::endl;
        }
        else if (valread == 0)
        {
            std::cout << "Server closed the connection" << std::endl;
            break;
        }
        else
        {
            std::cerr << "Failed to receive message from server" << std::endl;
            break;
        }
    }

    close(sock);
}
