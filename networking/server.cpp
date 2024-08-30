#include <iostream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Data
{
public:
    int health;
    char name[50];
};

void handle_client(int client_socket)
{
    char buffer[1024] = {0};

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(client_socket, buffer, sizeof(buffer) - 1);
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
        std::cout << "Message received: " << buffer << std::endl;

        if (strcmp(buffer, "SHUTDOWN") == 0)
            break;

        const char *confirmation = "Message Received";
        send(client_socket, confirmation, strlen(confirmation), 0);
    }

    close(client_socket);
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            close(server_fd);
            return -1;
        }

        std::thread client_thread(handle_client, new_socket);
        client_thread.detach();
    }

    close(server_fd);
    return 0;
}
