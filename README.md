# Project Overview

This project consists of a server and a client application designed to facilitate real-time chat communication between multiple clients. It is implemented in C++ using the Winsock2 API for network communication. The server handles multiple clients simultaneously, relaying messages between them, while each client connects to the server to send and receive messages.

**Project Steps**
* Initialize Winsock
* Server setup
* Client setup
* Message handling

# Local Setup

## Installation

To follow this project, we will need to install the following locally:

* A C++ compiler (e.g., GCC, MSVC)
* Winsock library (typically included with Windows development environments)

# Usage
Starting the Server:

* Compile and run the server code on a machine with network access.
* The server will start listening on port 12345 for incoming client connections.
  
Connecting Clients:

* Compile and run the client code on any machine.
* Provide the server's IP address (default is "127.0.0.1" for localhost) and connect.
* Enter a chat name and start sending and receiving messages.

Example Interaction:

* Client A connects to the server and sets the chat name as "Alice".
* Client B connects and sets the chat name as "Bob".
* Alice sends a message: "Hello everyone!"
* The server receives Alice’s message and forwards it to all connected clients, including Bob.
* Bob sees the message: "Alice : Hello everyone!" and responds with "Hi Alice!"
* The server forwards Bob's message to Alice and any other connected clients.
