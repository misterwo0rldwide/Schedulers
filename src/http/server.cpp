#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "server.h"

HttpServer::HttpServer() {
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(this->serverSocket, 5);
}

HttpServer::~HttpServer() {
    if (this->serverSocket)
        close(this->serverSocket);
}

void HttpServer::recvClients() {
    std::cout << "Waiting for clients to connect on 127.0.0.1:8080" << std::endl;
    int clientSocket = accept(this->serverSocket, nullptr, nullptr);

    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
              << std::endl;

    // closing the socket.
    close(serverSocket);
}

void HttpServer::handleClient(int clientSocket) {
}