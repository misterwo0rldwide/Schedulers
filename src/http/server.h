#pragma once

#define MAX_LENGTH (1024)

#define SERVER_PORT (8080)
#define SERVER_LISTEN (5)

class HttpServer {
    private:
        bool serverOn;
        int serverSocket;
    public:
        HttpServer();
        ~HttpServer();

        void recvClients();
    private:
        void handleClient(int clientSocket);
        std::string handleClientRequest(std::string request);
};