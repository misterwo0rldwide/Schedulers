#include <string>
#include <thread>
#include <iostream>

#include "server.h"
#include "tcp/tcpSocket.h"

HttpServer::HttpServer() {
    this->serverOn = true;
    this->serverSocket = createServerSocket(SERVER_PORT, SERVER_LISTEN);
}

HttpServer::~HttpServer() {
    if (this->serverSocket >= 0)
        close(this->serverSocket);

    std::cout << "Closing server" << std::endl;
}

void HttpServer::recvClients() {
    int clientSocket;

    std::cout << "Waiting for clients to connect on 127.0.0.1:8080" << std::endl;
    while (this->serverOn) {
        clientSocket = accept(this->serverSocket, nullptr, nullptr);
        
        std::thread t(this->handleClient, clientSocket);
        t.detach();
    }
}

void HttpServer::handleClient(int clientSocket) {
    std::string request  = recvBySizeHttp(clientSocket);
    std::string response = this->handleClientRequest(request);

    sendBySizeHttp(response);

    // Http is connectionless
    close(clientSocket);
}

std::string handleClientRequest(const std::string& request) {
    std::string response;

    // Http server logic
}