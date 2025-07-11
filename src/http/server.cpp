#include <string>
#include <cstring>
#include <thread>
#include <fstream>
#include <sstream>
#include <iostream>

#include "server.h"
#include "httpHeaders.h"
#include "tcp/tcpSocket.h"

HttpServer::HttpServer() {
    this->serverOn = true;
    this->serverSocket = createServerSocket(SERVER_PORT, SERVER_LISTEN);
}

HttpServer::~HttpServer() {
    if (this->serverSocket >= 0)
        closeSocket(this->serverSocket);

    std::cout << "Closing server" << std::endl;
}

void HttpServer::recvClients() {
    int clientSocket;

    std::cout << "Waiting for clients to connect on 127.0.0.1:8080" << std::endl;
    while (this->serverOn) {
        clientSocket = acceptClientSocket(this->serverSocket);
        
        std::thread t([this, clientSocket]() { this->handleClient(clientSocket);});
        t.detach();
    }
}

void HttpServer::handleClient(int clientSocket) {
    std::string request  = recvBySizeHttp(clientSocket);
    std::string response = this->handleClientRequest(request);

    sendBySizeHttp(clientSocket, response);

    // Http is connectionless
    closeSocket(clientSocket);
}

std::string HttpServer::handleClientRequest(const std::string& request) {
    std::string requestPath = this->getRequestedPath(request);
    
    // Only supporting GET/POST
    if (request.rfind("GET", 0) == 0)
        return this->HandleGetRequest(requestPath);
    
    //return this->HandlePostRequest(requestPath, request);
}

std::string HttpServer::HandlePostRequest(const std::string& requestPath, const std::string& request) {
    std::string contentLength, contentType, body;

    try {

    } catch(...) {
        return HTTP_500_INTERNAL_ERROR_TEXT;
    }

    contentLength = LENGTH_HEADER + std::to_string(body.size()) + HEADER_END;
    return HTTP_OK + contentType + contentLength + ACCESS_CONTROL_ALLOW + HEADER_END + body;
}

std::string HttpServer::HandleGetRequest(std::string& requestPath) {
    std::string contentLength, contentType, body;
    
    try {
        // Check if file is found inside allowed directory
        if (requestPath == "/") {
            requestPath = DEFAULT_PATH;
        } else
            requestPath.erase(0, 1); // remove '/' at start

        if (requestPath.rfind("api", 0) == 0) {
            contentType = CONTENT_TYPE_JSON;

            if (requestPath.find("health") != std::string::npos)
                body = API_STATUS_OK;
            else if (requestPath.find("state") != std::string::npos)
                body = "hello";//getJsonSchedState();
        }
        else {
            if (!this->fileNameInsideDir(requestPath, ALLOWED_ROOT)) {
                return HTTP_404_NOT_FOUND_TEXT;
            }
            
            // Find file extension
            const char* dot = strstr(requestPath.c_str(), ".");
            if (!dot) return HTTP_404_NOT_FOUND_TEXT;

            const char* pathExtension = dot + 1;
            if (!strcmp(pathExtension, "txt"))
                contentType = CONTENT_TYPE_TXT;
            else if (!strcmp(pathExtension, "html"))
                contentType = CONTENT_TYPE_HTML;
            else if (!strcmp(pathExtension, "json"))
                contentType = CONTENT_TYPE_JSON;
            else if (!strcmp(pathExtension, "jpeg"))
                contentType = CONTENT_TYPE_JPEG;
            else if (!strcmp(pathExtension, "png"))
                contentType = CONTENT_TYPE_PNG;
            else if (!strcmp(pathExtension, "css"))
                contentType = CONTENT_TYPE_CSS;
            else if (!strcmp(pathExtension, "js"))
                contentType = CONTENT_TYPE_JS;
            else if (!strcmp(pathExtension, "ico"))
                contentType = CONTENT_TYPE_ICO;
            else
                return HTTP_404_NOT_FOUND_TEXT;
            
            // Get request path content
            std::ifstream requestFile(ALLOWED_ROOT + requestPath, std::ios::binary);
            if (!requestFile)
                return HTTP_500_INTERNAL_ERROR_TEXT;
            
            // Copy path content to buffer
            std::stringstream buffer;
            buffer << requestFile.rdbuf();
            body = buffer.str();
        }

    } catch(...) {
        return HTTP_500_INTERNAL_ERROR_TEXT;
    }

    contentLength = LENGTH_HEADER + std::to_string(body.size()) + HEADER_END;
    return HTTP_OK + contentType + contentLength + ACCESS_CONTROL_ALLOW + HEADER_END + body;
}

std::string HttpServer::getRequestedPath(const std::string& request) {
    size_t firstSpace = request.find(' ');
    if (firstSpace == std::string::npos) return "";

    size_t secondSpace = request.find(' ', firstSpace + 1);
    if (secondSpace == std::string::npos) return "";

    return request.substr(firstSpace + 1, secondSpace - firstSpace - 1);
}

bool HttpServer::fileNameInsideDir(const std::string& requestedPath, const std::string& allowedRoot) {
    try {
        std::filesystem::path allowedRootPath(allowedRoot);
        std::filesystem::path fullPath = allowedRootPath / requestedPath;
        std::cout << "Path is " << fullPath << std::endl;

        std::filesystem::path canonicalRequested = std::filesystem::canonical(fullPath);
        std::filesystem::path canonicalRoot = std::filesystem::canonical(allowedRootPath);

        return std::mismatch(
            canonicalRoot.begin(), canonicalRoot.end(),
            canonicalRequested.begin()
        ).first == canonicalRoot.end();
    } catch (...) {
        return false;
    }
}

