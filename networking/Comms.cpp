#include "Comms.h"

Comms::Comms() : sock(-1) {}

void Comms::create_socket()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
}

void Comms::close_socket()
{
    if (sock >= 0)
    {
        close(sock);
    }
}

int Comms::send_message(const std::string &message)
{
    int result = send(sock, message.c_str(), message.length(), 0);
    return result;
}

int Comms::receive_message(char *buffer, int buffer_size)
{
    int valread = recv(sock, buffer, buffer_size - 1, 0);
    buffer[valread] = '\0';
    return valread;
}

Comms::~Comms()
{
    close_socket();
}
