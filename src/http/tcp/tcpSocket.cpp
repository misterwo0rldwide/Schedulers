#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <errno.h>
#include <cstring>
#include <unistd.h>

#include "tcpSocket.h"
#include "../httpHeaders.h"

int createServerSocket(int port, int serverListen) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(sock);
        return -1;
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        close(sock);
        return -1;
    }

    if (listen(sock, serverListen) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

int acceptClientSocket(int sock) {
    return accept(sock, nullptr, nullptr);
}

ssize_t sendBySizeHttp(int sock, const std::string& response) {
    size_t totalSent = 0;
    const char* data = response.data();
    size_t remaining = response.size();

    while (remaining > 0) {
        size_t chunkSize = std::min(remaining, static_cast<size_t>(PACKET_BLOCK_SIZE));
        ssize_t bytesSent = send(sock, data + totalSent, chunkSize, MSG_NOSIGNAL);

        if (bytesSent < 0) {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return -1;
        }
        
        if (bytesSent == 0) {
            std::cerr << "Connection closed by peer during send" << std::endl;
            return -1;
        }
        
        totalSent += bytesSent;
        remaining -= bytesSent;
    }
    
    return static_cast<ssize_t>(totalSent);
}

std::string recvBySizeHttp(int sock) {
    char headersBuf[PACKET_BLOCK_SIZE * 2] = { 0 };
    ssize_t headersLength = 0, tmpLength = 0;

    while (headersLength < static_cast<ssize_t>(sizeof(headersBuf))) {
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

void closeSocket(int sock) {
    if (sock)
        close(sock);
}