#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "tcpSocket.h"
#include "../httpHeaders.h"

int createServerSocket(int port, int serverListen) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(this->serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(this->serverSocket, serverListen);
    
    return sock;
}

size_t sendBySizeHttp(int sock, const std::string& response) {
    size_t reponseLength = 0;
    size_t tmpLength;

    while (reponseLength < response.size()) {
        tmpLength = send(sock, response.data() + reponseLength, PACKET_BLOCK_SIZE, 0);

        if (tmpLength <= 0)
            return -1;
        
        reponseLength += tmpLength;
    }
    return reponseLength;
}

std::string recvBySizeHttp(int sock) {
    char headersBuf[PACKET_BLOCK_SIZE * 2] = { 0 };
    ssize_t headersLength = 0, tmpLength = 0;

    while (headersLength < sizeof(headersBuf)) {
        tmpLength = recv(sock, headersBuf + headersLength, PACKET_BLOCK_SIZE, 0);

        if (tmpLength <= 0)
            return "";

        headersLength += tmpLength;
        if (strstr(headersBuf + std::max<ssize_t>(0, headersLength - tmpLength - 4), END_HEADERS))
            break;
    }

    std::string response(headersBuf, headersLength);

    // Find Content-Length header
    char* contentLengthPos = strstr(headersBuf, LENGTH_HEADER);
    if (!contentLengthPos)
        return response;

    contentLengthPos += strlen(LENGTH_HEADER);
    size_t dataLength;
    try
    {
        dataLength = std::stoul(contentLengthPos);
    } catch(...) {
        std::cout << "Corrupt Content Length header" << std::endl;
        return "";
    }
    if (dataLength > MSG_MAX_SIZE)
        return "";

    // Find where the body starts
    char* bodyStart = strstr(headersBuf, END_HEADERS);
    if (!bodyStart)
        return response;

    bodyStart += strlen(END_HEADERS);
    size_t bodyBytesAlreadyRead = headersLength - (bodyStart - headersBuf);

    std::string body(bodyStart, bodyBytesAlreadyRead);

    // Read the rest of the body
    while (body.size() < dataLength) {
        char buf[PACKET_BLOCK_SIZE];
        ssize_t n = recv(sock, buf, PACKET_BLOCK_SIZE, 0);
        if (n <= 0)
            return "";
        body.append(buf, n);
    }

    return response + body;
}