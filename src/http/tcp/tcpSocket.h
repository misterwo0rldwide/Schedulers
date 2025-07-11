#pragma once

#include <string>

#define PACKET_BLOCK_SIZE (1024 * 4)
#define MSG_MAX_SIZE (PACKET_BLOCK_SIZE * PACKET_BLOCK_SIZE)

//int createClientSocket()
int createServerSocket(int port, int serverListen);
int acceptClientSocket(int sock);
ssize_t sendBySizeHttp(int sock, const std::string& str);
std::string recvBySizeHttp(int sock);
void closeSocket(int sock);