# Networking Project

This project demonstrates a basic client-server networking application using C++ on a UNIX-based system. The server listens for incoming connections, and the client sends messages to the server. Both the server and the client are designed to run on separate machines or on the same machine using configurable IP addresses and ports.

## Features

- **Client-Server Communication:** The client can send messages to the server, and the server can respond back.
- **Configurable IP and Port:** Both the server and the client can be configured to use different IP addresses and ports.
- **Error Handling:** The application includes robust error handling for socket creation, binding, connecting, and communication errors.
- **Multithreading:** The server handles multiple clients simultaneously using threads.
- **Graceful Shutdown:** The client can terminate the connection by sending a "QUIT" message.

## Requirements

- **Operating System:** UNIX-based OS (e.g., macOS, Linux)
- **Compiler:** g++ or clang++ with C++11 support

## File Structure

- **`Client.h` and `Client.cpp`:** Contains the client-side implementation.
- **`Server.h` and `Server.cpp`:** Contains the server-side implementation.
- **`Comms.h` and `Comms.cpp`:** Contains common communication utilities used by both the client and server.
- **`main.cpp`:** The entry point for the application, which determines whether to compile and run as a client or server.
- **`README.md`:** This file, which explains how to set up, compile, and run the project.

## Setup and Compilation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/networking-project.git
cd networking-project
```

### 2. Compilation

This project can be compiled to either run as a server or a client. By default, it compiles as a client. Use the following commands to compile:

- **Compile as Client (Default):**

```bash
c++ main.cpp Client.cpp Comms.cpp -std=c++11 -o client
```

- **Compile as Server:**

```bash
c++ main.cpp Server.cpp Comms.cpp -std=c++11 -DCOMPILE_AS_SERVER -o server
```

### 3. Running the Application

#### Running the Server

To run the server, use the following command. By default, the server listens on port 8080, but you can specify a different port.

```bash
./server [port]
```

##### Example (default port 8080):

```bash
./server 8080
```

#### Running the Client

To run the client, use the following command. The client connects to the specified server IP and port. By default, it connects to localhost on port 8080.

```bash
./client [server_ip] [port]
```

##### Example (default port 8080):

```bash
./client 127.0.0.1 8080
```

## Debug Mode

This project includes optional debug prints that can help you understand the internal processes of the client-server communication, such as socket creation, connection status, timeouts, and message exchanges. To enable debug messages, you can compile both the client and server with the `-DDEBUG` flag.

### **Compile the Client in Debug Mode**

To compile the client with debug information:

```bash
c++ main.cpp Client.cpp Comms.cpp -std=c++11 -o client -DDEBUG
```

This will include additional debug prints in the output, such as:

- Socket creation
- Timeout settings
- TCP keep-alive settings
- Sent and received messages

### **Compile the Server in Debug Mode**

To compile the server with debug information:

```bash
c++ main.cpp Server.cpp Comms.cpp -std=c++11 -DCOMPILE_AS_SERVER -o server -DDEBUG
```

This will include debug messages showing:

- Client connection and disconnection events
- TCP keep-alive settings for each client
- Sent and received messages for each client
