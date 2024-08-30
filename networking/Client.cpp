#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    std::string message;
    while (true)
    {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        send(sock, message.c_str(), message.length(), 0);

        if (message == "SHUTDOWN")
            break;

        memset(buffer, 0, sizeof(buffer));

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
    }

    close(sock);
    return 0;
}
