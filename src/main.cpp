#include <chrono>
#include <iostream>
#include <thread>

#include "http_server/httpServer.hpp"

int main()
{
    {
        http::TcpServer server = http::TcpServer("192.168.0.51", 2137);
        std::thread server_thread([&server] { server.startListen(); });
        char option;
        server_thread.detach();
        while (option != 'q')
        {
            std::cin >> option;
        }
        std::cout << "Close server\n";
        server.endListening();
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
