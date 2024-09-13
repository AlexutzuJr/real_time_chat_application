#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")


bool initialize()
{
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMsg(SOCKET s) 
{
    std::cout << "Enter your chat name: ";
    std::string name;
    std::getline(std::cin, name);
    std::string message;

    while (1) 
    {
        std::getline(std::cin, message);
        std::string msg = name + " : " + message; 
        int bytessent = send(s, msg.c_str(), msg.length(), 0); 

        if (bytessent == SOCKET_ERROR) 
        {
            std::cout << "Error sending message" << std::endl;
            break;
        }

        if (message == "quit")
        {
            std::cout << "Stopping the application" << std::endl;
            break;
        }
    }

    closesocket(s);
    WSACleanup();
}

void ReceiveMsg(SOCKET s) 
{
    char buffer[4096]; 
    int receivelength; 
    std::string message = ""; 

    while (1) 
    {
        receivelength = recv(s, buffer, sizeof(buffer), 0);

        if (receivelength <= 0) 
        {
            std::cout << "Disconnected from the server" << std::endl;
            break;
        }
        else
        {
            message = std::string(buffer, receivelength); 
            std::cout << message << std::endl;
        }
    }

    closesocket(s);
    WSACleanup();
}


int main()
{
    if (!initialize()) 
    {
        std::cout << "Initialize winsock failed" << std::endl;

        return 1;
    }

    SOCKET s;  
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == INVALID_SOCKET)
    {
        std::cout << "Invalid socket created" << std::endl;
        return 1;
    }


    int port = 12345;
    std::string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr)); 

    if (connect(s, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
    {
        std::cout << "Not able to connect to server" << std::endl; 
        std::cout << ": " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Successfully connected to server" << std::endl;


    std::thread senderthread(SendMsg, s); 
    std::thread receiver(ReceiveMsg, s);

    senderthread.join(); 
    receiver.join();


    std::string message = "Hello there";
    int bytessent;

    bytessent = send(s, message.c_str(), message.length(), 0); 

    if (bytessent == SOCKET_ERROR)
    {
        std::cout << "Send failed" << std::endl;
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
