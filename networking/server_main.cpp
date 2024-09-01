#include "Server.h"

int main()
{
    Server server;
    server.create_socket();
    server.run();
}
