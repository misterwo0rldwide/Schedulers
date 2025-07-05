#pragma once

#include <string>

#define PACKET_BLOCK_SIZE (1024 * 4)
#define MSG_MAX_SIZE (PACKET_BLOCK_SIZE * PACKET_BLOCK_SIZE)

//int createClientSocket()
int createServerSocket(int port, int serverListen);
int sendBySizeHttp(int sock, std::string);
char* recvBySizeHttp(int sock)