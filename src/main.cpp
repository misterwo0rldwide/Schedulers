#include <string>
#include <iostream>

#include "http/server.h"

int main(void) {
    HttpServer server;

    server.recvClients();
    return 0;
}