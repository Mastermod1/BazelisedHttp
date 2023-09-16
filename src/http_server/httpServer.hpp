#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <string>

namespace http
{
class TcpServer
{
  public:
    TcpServer(std::string ip_address, int port);
    ~TcpServer();

    void startListen();
    void endListening();

  private:
    std::string ip_address_;
    int port_;
    int socket_;
    int new_socket_;
    long incoming_message_;
    struct sockaddr_in socket_address_;
    unsigned int socket_address_length_;
    std::string server_message_;
    bool is_listening_ = true;

    int startServer();
    void closeServer();
    void acceptConnection(int &new_socket);
    std::string buildResponse();
    void sendResponse();
};
}  // namespace http
#endif
