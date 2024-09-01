#ifndef COMMS_H
#define COMMS_H

#include <exception>
#include <string>

class CommsException : public std::exception {
public:
    explicit CommsException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
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
};

#endif // COMMS_H
