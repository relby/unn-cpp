#pragma once

#include <netdb.h>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../common/socket.h"
#include "../common/utils.h"

const size_t MAX_RECV = 1024;

class Client : public Socket {
public:
    Client(const uint32_t port);
    int create_connection();
    int send(int socket_fd, const std::string &msg) override;
    int recv(int socket_fd, std::string &msg) override;
};

Client::Client(const uint32_t port) : Socket("localhost") {
    std::cout << "[INFO]: Initializing a client instance ..." << std::endl;
    std::cout << "[INFO]: Collecting host information ..." << std::endl;
    hostent *hp;
    if ((hp = gethostbyname(this->_hostname)) == NULL) {
        throw std::invalid_argument("[ERROR]: Failed to get host info");
    }
    memset(&this->_addr, 0, sizeof(this->_addr));
    memcpy((char *)&this->_addr.sin_addr, hp->h_addr,hp->h_length);
    this->_addr.sin_family = hp->h_addrtype;
    this->_addr.sin_port = htons(port);
    std::string server_ip;
    std::string server_port;
    parse_socket(&this->_addr, server_ip, server_port);
    std::cout <<"[INFO]: "<< "Host " << server_ip << " Port " << server_port << std::endl;
}

int Client::create_connection() {
    std::cout << "Client: creating client socket ..." << std::endl;
    if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Client: failed to create a socket");
        return -1;
    }
    std::cout << "Client: socket starts connecting to host ..." << std::endl;
    if (connect(this->_socket, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0) {
        this->close();
        perror("Client: socket failed to connect with host ...");
        return -1;
    }
    return 0;
}

int Client::send(int sockfd, const std::string &msg) {
    uint32_t total_sent = 0;
    uint32_t sent = 0;
    while (total_sent < msg.size()) {
        if ((sent = ::send(sockfd, msg.c_str() + total_sent, msg.size() - total_sent, 0)) < 0) {
            return -1;
        }
        total_sent += sent;
    }
    return 0;
}

int Client::recv(int sockfd, std::string &msg) {
    char buffer[MAX_RECV + 1];
    bool quit = false;
    while (!quit) {
        memset(buffer, 0, sizeof(buffer));
        int state = ::recv(sockfd, buffer, MAX_RECV, 0);
        if (state < 0) {
            std::cout << "[ERORR]: Failed to receive the echo message to client, closing socket" << std::endl;
            ::close(sockfd);
            return -1;
        } else if (state == 0) {
            std::cout << "[INFO]: Detected client closed, closing socket ..." << std::endl;
            ::close(sockfd);
            return -1;
        } else if (state < MAX_RECV) {
            quit = true;
        }
        msg.append(std::string(buffer));
    }
    std::cout << "[SERVER]: " << msg << std::endl;
    return 0;
}
