#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>  
#include <tchar.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib") 


bool initialize() 
{
    WSADATA data; 
    return WSAStartup(MAKEWORD(2, 2), &data) == 0; 
}

void InteractWithClient(SOCKET clientSocket, std::vector<SOCKET> &clients) 
{
    std::cout << "Client connected" << std::endl;
    char buffer[4096]; 

    while (1) 
    {
        int bytesreceived = recv(clientSocket, buffer, sizeof(buffer), 0); 

        if (bytesreceived <= 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::string message(buffer, bytesreceived); 
        std::cout << "Message from client: " << message << std::endl;

        for (auto client : clients) 
        {
            if (client != clientSocket) 
            {
                send(client, message.c_str(), message.length(), 0); 
            }
        }
    }
    auto it = find(clients.begin(), clients.end(), clientSocket); 

    if (it != clients.end()) 
    {
        clients.erase(it); 
    }

    closesocket(clientSocket); 
}


int main()
{
    if (!initialize()) 
    {
        std::cout << "Winsock initialization failed" << std::endl;
        return 1; 
    };

    std::cout << "Server program" << std::endl;


    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); 

    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed" << std::endl;
        return 1;
    }

    
    int port = 12345;
    sockaddr_in serveraddr; 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_port = htons(port); 

    if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) 
    {
        std::cout << "Setting address structure failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (bind(listenSocket, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) 
    {
        std::cout << "Bind failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
        std::cout << "Listen failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server has started listening on port: " << port << std::endl;


    std::vector<SOCKET> clients;

    while (1) 
    {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "Invalid client socket" << std::endl;
        }

        clients.push_back(clientSocket); 
        std::thread t1(InteractWithClient, clientSocket, std::ref(clients)); 
        t1.detach(); 
    }

    closesocket(listenSocket); 
    WSACleanup(); 

    return 0;
}
