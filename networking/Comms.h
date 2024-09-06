#ifndef COMMS_H
#define COMMS_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <regex>

#ifdef DEBUG
    #define DEBUG_PRINT(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif

class CommsException : public std::runtime_error {
public:
    explicit CommsException(const std::string& message) : std::runtime_error(message) {}
};

class Comms {
protected:
    int sock;

public:
    Comms();
    void create_socket();
    void close_socket();
    int send_message(const std::string &message);
    int receive_message(char *buffer, int buffer_size);
    virtual ~Comms();

    // Utility methods for validation
    bool is_valid_ip(const std::string& ip);
    bool is_valid_port(int port);
};

#endif // COMMS_H
