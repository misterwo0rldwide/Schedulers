#include <string>
#include <iostream>

#include "http/server.h"

int main(void) {
    HttpServer server;

    server.recvClients();
    std::cout << "Bye Bye" << std::endl;

    return 0;
}