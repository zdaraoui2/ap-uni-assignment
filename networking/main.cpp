#include "Client.h"
#include "Server.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    #ifndef COMPILE_AS_SERVER
        // Default values for client
        std::string server_ip = "127.0.0.1"; // Default to localhost
        int port = 8080;                    // Default to port 8080

        // Override defaults if arguments are provided
        if (argc >= 2) {
            server_ip = argv[1];
        }
        if (argc >= 3) {
            port = std::stoi(argv[2]);
        }

        Client client;
        client.create_socket();
        client.connect_to_server(server_ip, port);
        client.run();
    #else
        // Default values for server
        int port = 8080; // Default to port 8080

        // Override default if argument is provided
        if (argc >= 2) {
            port = std::stoi(argv[1]);
        }

        Server server;
        server.create_socket();
        server.run(port);
    #endif

    return 0;
}
