#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

#include <string>

class SocketException {
private:
    std::string _message;
public:
    SocketException(const std::string& message);

    std::string message() const;
};

SocketException::SocketException(const std::string& message)
    : _message(message)
{}

std::string SocketException::message() const {
    return this->_message;
};


#endif // SOCKET_EXCEPTION_H
