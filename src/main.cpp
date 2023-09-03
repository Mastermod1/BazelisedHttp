#include <iostream>

#include "http_server/httpServer.hpp"

int main()
{
    http::TcpServer server = http::TcpServer("192.168.1.35", 2137);
    server.startListen();
    return 0;
}
