#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "socket.h"
#include "socket_exception.h"

class ClientSocket : private Socket {
public:
    ClientSocket(const std::string& host, const int port);

    const ClientSocket& operator<<(const std::string& message) const;
    const ClientSocket& operator>>(std::string& message) const;
};

ClientSocket::ClientSocket(const std::string& host, const int port) {
    if (!Socket::create()) {
        throw SocketException("Could not create client socket");
    }
    if (!Socket::connect(host, port)) {
        throw SocketException("Could not bind to port");
    }
}

const ClientSocket& ClientSocket::operator<<(const std::string& message) const {
    if (!Socket::send(message)) {
        throw SocketException("Could not write to socket");
    }
    return *this;
}

const ClientSocket& ClientSocket::operator>>(std::string& message) const {
    if (!Socket::recv(message)) {
        throw SocketException("Could not read from socket");
    }
    return *this;
}

#endif // CLIENT_SOCKET_H
