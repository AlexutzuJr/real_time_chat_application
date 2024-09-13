#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>  
#include <tchar.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib") //ofera functii pentru a crea, lega, asculta, accepta, trimite si primi date prin socket-uri de retea


bool initialize() //initializam biblioteca "Winsock"
{
    WSADATA data; //structura "WSADATA" contine informatii despre implementarea Windows Sockets
    return WSAStartup(MAKEWORD(2, 2), &data) == 0; //MAKEWORD creeaza o valoare de 16 biti din 2 valori de 8 biti (solicitand versiunea 2.2 a Winsock)
}

void InteractWithClient(SOCKET clientSocket, std::vector<SOCKET>& clients) //facilitam comunicarea dintre server si client, gestionand primirea si difuzarea mesajelor catre alti clienti 
{
    std::cout << "Client connected" << std::endl;
    char buffer[4096]; //stocam datele primite de la client

    while (1) //intram intr-o bucla infinita pentru a primi si procesa continuu date de la client
    {
        int bytesreceived = recv(clientSocket, buffer, sizeof(buffer), 0); //primeste date de la socket-ul clientului; "recv" returneaza numarul de octeti primiti

        if (bytesreceived <= 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        std::string message(buffer, bytesreceived); //construim un string din buffer-ul primit, folosind numarul de octeti primiti 
        std::cout << "Message from client: " << message << std::endl;

        for (auto client : clients) //iteram peste toti clientii conectati
        {
            if (client != clientSocket) //ne asiguram ca mesajul nu este trimis inapoi expeditorului initial
            {
                send(client, message.c_str(), message.length(), 0); //trimitem mesajul altor clienti
            }
        }
    }
    auto it = find(clients.begin(), clients.end(), clientSocket); //cautam pozitia "clientSocket" in vectorul clienti

    if (it != clients.end()) //verificam daca socket-ul clientului a fost gasit in lista
    {
        clients.erase(it); //daca este gasit, il eliminam din vectorul clienti
    }

    closesocket(clientSocket); //inchidem socket-ul asociat cu clientul pentru a curata resurse
}


int main()
{
    if (!initialize()) //apelam functia de initializare pentru a initializa Winsock
    {
        std::cout << "Winsock initialization failed" << std::endl;
        return 1; //daca initializarea esueaza, este afisat un mesaj de eroare si returneaza 1, indicand o eroare
    };

    std::cout << "Server program" << std::endl;


    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); //cream un socket cu AF_INET (IPv4), SOCK_STREAM (TCP) si 0 (protocol implicit)

    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "Socket creation failed" << std::endl;
        return 1;
    }

    //configuram adresa serverului
    int port = 12345;
    sockaddr_in serveraddr; //declaram o variabila numita "serveraddr" de tip "sockaddr_in"; "sockaddr_in" este o structura definita in "winsock2.h" utilizata pentru adresele IPv4
    serveraddr.sin_family = AF_INET; //seteaza membrul "sin_family" al structurii "serveraddr" la AF_INET, care este o constanta ce specifica familia de adrese pentru adresele IPv4
    serveraddr.sin_port = htons(port); //seteaza numarul portului pentru socket-ul serverului, transformandu-l din ordinea octetilor gazdei, in ordinea octetilor de retea

    if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) //convertim adresa ip (0.0.0.0) in format binar; daca setarea adresei esueaza..
    {
        std::cout << "Setting address structure failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (bind(listenSocket, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) //legam socket-ul la adresa serverului
    {
        std::cout << "Bind failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) //setam socket-ul sa asculte solicitarile de conectare primite; SOMAXCONN specifica lungimea maxima pentru coada de conexiuni in asteptare 
    {
        std::cout << "Listen failed" << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server has started listening on port: " << port << std::endl;


    std::vector<SOCKET> clients;

    while (1) //intram intr-o bucla infinita pentru a accepta (continuu) noi conexiuni client
    {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cout << "Invalid client socket" << std::endl;
        }

        clients.push_back(clientSocket); //adaugam un nou client socket la vectorul clienti
        std::thread t1(InteractWithClient, clientSocket, std::ref(clients)); //cream un nou thread pentru a gestiona comunicarea cu clientul
        t1.detach(); //detasam thread-ul pentru a-i permite sa ruleze independent
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}