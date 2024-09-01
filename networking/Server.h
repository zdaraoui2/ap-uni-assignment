#ifndef SERVER_H
#define SERVER_H

#include "Comms.h"

class Server : public Comms
{
public:
    void bind_socket();
    void listen_for_connections();
    int accept_connection();
    void handle_client(int client_socket);
    void run();
};

#endif // SERVER_H
