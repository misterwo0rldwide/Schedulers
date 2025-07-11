#pragma once

#include <filesystem>

#define MAX_LENGTH (1024)
#define SERVER_PORT (8080)
#define SERVER_LISTEN (5)

// Executable is inside build
#define ALLOWED_ROOT "../src/http/static/"

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
        std::string handleClientRequest(const std::string& request);
        std::string getRequestedPath(const std::string& request);
        std::string HandleGetRequest(std::string& request);
        std::string HandlePostRequest(const std::string& requestPath, const std::string& request);
        bool fileNameInsideDir(const std::string& requestedPath, const std::string& allowedRoot);
};