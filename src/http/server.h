#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <string>

class HttpServer {
    private:
        int serverSocket;
        std::vector<std::thread> clientsThreads;
    public:
        HttpServer();
        ~HttpServer();

        void recvClients();
    private:
        void handleClient(int clientSocket);
        std::string handleClientRequest(std::string request);
};