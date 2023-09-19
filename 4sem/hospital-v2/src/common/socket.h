#pragma once

#include <netdb.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <climits>
#include <map>
#include <mutex>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

const int MAX_HOST_NAME = 255;

class Socket {
protected:
    int _socket;
    sockaddr_in _addr;
    char _hostname[MAX_HOST_NAME + 1];
public:
    Socket();
    Socket(const char* hostname);
    virtual ~Socket();
    sockaddr_in get_socket_address() const;
    int get_socket_fd() const;
    void close();
    virtual int send(int socket_fd, const std::string &msg) = 0;
    virtual int recv(int socket_fd, std::string &msg) = 0;
};

sockaddr_in Socket::get_socket_address() const {
    return this->_addr;
}

int Socket::get_socket_fd() const {
    return this->_socket;
}

void Socket::close() {
    ::close(this->_socket);
}

Socket::Socket() {}

Socket::Socket(const char* hostname) {
    strcpy(this->_hostname, hostname);
}

Socket::~Socket() {
    this->close();
}
