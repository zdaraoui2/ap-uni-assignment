#include "Client.h"

int main()
{
    Client client;
    client.create_socket();
    client.connect_to_server();
    client.run();
}
