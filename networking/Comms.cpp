#include "Comms.h"
#include <regex>

Comms::Comms() : sock(-1) {}

void Comms::create_socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw CommsException("Socket creation failed: " + std::string(strerror(errno)));
    }
}

void Comms::close_socket() {
    if (sock >= 0) {
        if (close(sock) < 0) {
            throw CommsException("Socket close failed: " + std::string(strerror(errno)));
        }
        sock = -1;
    }
}

int Comms::send_message(const std::string &message) {
    int result = send(sock, message.c_str(), message.length(), 0);
    if (result < 0) {
        throw CommsException("Send message failed: " + std::string(strerror(errno)));
    }
    return result;
}

int Comms::receive_message(char *buffer, int buffer_size) {
    int valread = recv(sock, buffer, buffer_size - 1, 0);
    if (valread < 0) {
        throw CommsException("Receive message failed: " + std::string(strerror(errno)));
    }
    buffer[valread] = '\0';  // Null-terminate the received message
    return valread;
}

Comms::~Comms() {
    close_socket();
}

// IP validation
bool Comms::is_valid_ip(const std::string& ip) {
    std::regex ip_regex("^(\\d{1,3}\\.){3}\\d{1,3}$");
    return std::regex_match(ip, ip_regex);
}

// Port validation
bool Comms::is_valid_port(int port) {
    return port > 0 && port <= 65535;
}
