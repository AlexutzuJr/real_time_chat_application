# Project Overview

In this project, our goal is to predict tomorrow's temperature using historical data. The process begins by downloading a dataset of local weather. Next, the data will be cleaned and prepared for machine learning. A system will be built to make historical predictions, followed by the addition of more predictors to improve the model. We will end with how to make next-day predictions.

**Project Steps**
* Initialize Winsock
* Server setup
* Client setup
* Message handling

# Usage
Starting the Server:

* Compile and run the server code on a machine with network access.
* The server will start listening on port 12345 for incoming client connections.
  
Connecting Clients:

* Compile and run the client code on any machine.
* Provide the server's IP address (default is "127.0.0.1" for localhost) and connect.
* Enter a chat name and start sending and receiving messages.
* Example Interaction
* Client A connects to the server and sets the chat name as "Alice".
* Client B connects and sets the chat name as "Bob".
* Alice sends a message: "Hello everyone!"
* The server receives Alice’s message and forwards it to all connected clients, including Bob.
* Bob sees the message: "Alice : Hello everyone!" and responds with "Hi Alice!"
* The server forwards Bob's message to Alice and any other connected clients.

Example Interaction
* Client A connects to the server and sets the chat name as "Alice".
* Client B connects and sets the chat name as "Bob".
* Alice sends a message: "Hello everyone!"
* The server receives Alice’s message and forwards it to all connected clients, including Bob.
* Bob sees the message: "Alice : Hello everyone!" and responds with "Hi Alice!"
* The server forwards Bob's message to Alice and any other connected clients.
  
# Local Setup

## Installation

To follow this project, we will need to install the following locally:

* A C++ compiler (e.g., GCC, MSVC)
* Winsock library (typically included with Windows development environments)
