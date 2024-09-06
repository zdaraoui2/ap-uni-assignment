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
            try {
                port = std::stoi(argv[2]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid port number. Please provide a numeric port value." << std::endl;
                return -1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Port number out of range." << std::endl;
                return -1;
            }
        }

        Client client;

        try {
            if (!client.is_valid_ip(server_ip)) {
                std::cerr << "Invalid IP address format: " << server_ip << std::endl;
                return -1;
            }

            if (!client.is_valid_port(port)) {
                std::cerr << "Invalid port number: " << port << ". Port should be in the range 1-65535." << std::endl;
                return -1;
            }

            client.create_socket();
            client.connect_to_server(server_ip, port);
            client.run();
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return -1;
        }

    #else
        // Default values for server
        int port = 8080; // Default to port 8080

        // Override default if argument is provided
        if (argc >= 2) {
            try {
                port = std::stoi(argv[1]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid port number. Please provide a numeric port value." << std::endl;
                return -1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Port number out of range." << std::endl;
                return -1;
            }
        }

        Server server;

        try {
            if (!server.is_valid_port(port)) {
                std::cerr << "Invalid port number: " << port << ". Port should be in the range 1-65535." << std::endl;
                return -1;
            }

            server.create_socket();
            server.run(port);
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return -1;
        }

    #endif

    return 0;
}
