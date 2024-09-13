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

void SendMsg(SOCKET s) //cream o functie care sa permita utilizatorului, sa trimita mesaje catre un server sau un alt client printr-un socket
{
    std::cout << "Enter your chat name: ";
    std::string name;
    std::getline(std::cin, name);
    std::string message;

    while (1) //bucla de trimite a mesajelor
    {
        std::getline(std::cin, message);
        std::string msg = name + " : " + message; //formatam mesajul, astfel incat sa apara numele utilizatorului, urmat de mesajul trimis
        int bytessent = send(s, msg.c_str(), msg.length(), 0); //trimitem mesajul catre socket-ul "s"

        if (bytessent == SOCKET_ERROR) //daca trimiterea a esuat, este afisat un mesaj de eroare si iesim din bucla
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

void ReceiveMsg(SOCKET s) //primim si afisam mesaje de la server printr-un socket
{
    char buffer[4096]; //stocam mesajele primite
    int receivelength; //stocam numarul de octeti primiti de la socket
    std::string message = ""; //stocam mesajele

    while (1) //bucla pentru a continua primirea mesajelor
    {
        receivelength = recv(s, buffer, sizeof(buffer), 0);

        if (receivelength <= 0) //verificarea si afisarea mesajelor
        {
            std::cout << "Disconnected from the server" << std::endl;
            break;
        }
        else
        {
            message = std::string(buffer, receivelength); //daca datele au fost primite cu succes, le convertim intr-un string
            std::cout << message << std::endl;
        }
    }

    closesocket(s);
    WSACleanup();
}


int main()
{
    if (!initialize()) //apelam functia "initialize" pentru a initializa biblioteca Winsock
    {
        std::cout << "Initialize winsock failed" << std::endl;

        return 1;
    }

    SOCKET s;  //cream socket-ul pentru comunicarea TCP/IP(IPv4)
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == INVALID_SOCKET)
    {
        std::cout << "Invalid socket created" << std::endl;
        return 1;
    }

    //configuram adresa server-ului
    int port = 12345;
    std::string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr)); //convertim adresa IP in binar

    if (connect(s, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
    {
        std::cout << "Not able to connect to server" << std::endl; //daca conexiunea esueaza este afisat mesajul de eroare si se curata Winsock
        std::cout << ": " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }

    std::cout << "Successfully connected to server" << std::endl;


    std::thread senderthread(SendMsg, s); //lanseaza un thread care permite utilizatorului sa trimita mesaje
    std::thread receiver(ReceiveMsg, s);

    senderthread.join(); //asteptam ca thread-ul "senderthread" sa finalizeze executia, inainte de a continua executia principala a programului
    receiver.join();


    std::string message = "Hello there";
    int bytessent;

    bytessent = send(s, message.c_str(), message.length(), 0); //trimite un mesaj de test prin socket 

    if (bytessent == SOCKET_ERROR)
    {
        std::cout << "Send failed" << std::endl;
    }

    closesocket(s);
    WSACleanup();

    return 0;
}