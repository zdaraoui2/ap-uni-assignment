#include "Client.h"
#include "Server.h"

int main() {
    #ifndef COMPILE_AS_SERVER
        // Default to client if no flag is provided
        Client client;
        client.create_socket();
        client.connect_to_server();
        client.run();
    #else
        // Compile as server if the COMPILE_AS_SERVER flag is set
        Server server;
        server.create_socket();
        server.run();
    #endif

    return 0;
}