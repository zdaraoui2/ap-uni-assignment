#ifndef CLIENT_H
#define CLIENT_H

#include "Comms.h"

class Client : public Comms
{
public:
    void connect_to_server(const char *ip_address, int port);
    void run();
};

#endif // CLIENT_H
