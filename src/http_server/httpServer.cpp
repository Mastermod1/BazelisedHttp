#include "httpServer.hpp"

#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

namespace {
const int BUFFER_SIZE = 30720;

void log(const std::string& msg) { std::cout << msg << "\n"; }

void exitWithError(const std::string& errorMsg) {
    log("Error: " + errorMsg);
    exit(1);
}
}  // namespace

namespace http {
TcpServer::TcpServer(std::string ip_address, int port)
    : ip_address_(ip_address),
      port_(port),
      socket_(),
      new_socket_(),
      incoming_message_(),
      socket_address_(),
      socket_address_length_(sizeof(socket_address_)),
      server_message_(buildResponse()) {
    socket_address_.sin_family = AF_INET;
    socket_address_.sin_port = htons(port_);
    socket_address_.sin_addr.s_addr = inet_addr(ip_address_.c_str());

    if (startServer() != 0) {
        std::ostringstream ss;
        ss << "Failed to start server with PORT: " << ntohs(socket_address_.sin_port);
        log(ss.str());
    }
}

TcpServer::~TcpServer() { closeServer(); }

int TcpServer::startServer() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        exitWithError("Cannot create socket");
        return 1;
    }

    if (bind(socket_, (sockaddr*)&socket_address_, socket_address_length_) < 0) {
        exitWithError("Cannot connect socket to address");
        return 1;
    }

    return 0;
}

void TcpServer::closeServer() {
    close(socket_);
    close(new_socket_);
    exit(0);
}

void TcpServer::startListen() {
    if (listen(socket_, 20) < 0) {
        exitWithError("Socket listen failed");
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(socket_address_.sin_addr)
       << " PORT: " << ntohs(socket_address_.sin_port) << " ***\n\n";
    log(ss.str());

    int bytesReceived;
    while (true) {
        log("====== Waiting for a new connection ======\n\n\n");
        acceptConnection(new_socket_);

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(new_socket_, buffer, BUFFER_SIZE);
        if (bytesReceived < 0) {
            exitWithError("Failed to read bytes from client socket connection");
        }

        std::ostringstream ss;
        ss << "------ Received Request from client ------\n\n";
        log(ss.str());

        sendResponse();

        close(new_socket_);
    }
}

void TcpServer::acceptConnection(int& new_socket) {
    new_socket = accept(socket_, (sockaddr*)&socket_address_, &socket_address_length_);
    if (new_socket < 0) {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(socket_address_.sin_addr)
           << "; PORT: " << ntohs(socket_address_.sin_port);
        exitWithError(ss.str());
    }
}

std::string TcpServer::buildResponse() {
    std::string htmlFile =
        "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;

    return ss.str();
}

void TcpServer::sendResponse() {
    long bytesSent;

    bytesSent = write(new_socket_, server_message_.c_str(), server_message_.size());

    if (bytesSent == server_message_.size()) {
        log("------ Server Response sent to client ------\n\n");
    } else {
        log("Error sending response to client");
    }
}
}  // namespace http
