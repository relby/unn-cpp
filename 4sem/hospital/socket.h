#ifndef SOCKET_H
#define SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket {
private:
    int _sock;
    sockaddr_in _addr;
public:
    Socket();
    virtual ~Socket();

    bool create();
    bool bind(const int port);
    bool listen() const;
    bool accept(Socket& socket) const;

    bool connect(const std::string& host, const int port);

    bool send(const std::string& message) const;
    int recv(std::string&) const;

    bool is_valid() const;
};

Socket::Socket() : _sock(-1) {
    memset(&this->_addr, 0, sizeof(this->_addr));
}

Socket::~Socket() {
    if (this->is_valid()) {
        close(this->_sock);
    }
}

bool Socket::create() {
    this->_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!this->is_valid()) {
        std::cout << "not valid\n";
        return false;
    }

    int on = 1;
    int status = setsockopt(
        this->_sock,
        SOL_SOCKET,
        SO_REUSEADDR,
        (const char*)&on,
        sizeof(on)
    );

    return status != -1;
}

bool Socket::bind(const int port) {
    if (!this->is_valid()) return false;

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_addr.s_addr = INADDR_ANY;
    this->_addr.sin_port = htons(port);

    int status = ::bind(
        this->_sock,
        (struct sockaddr*)&this->_addr,
        sizeof(this->_addr)
    );

    return status != -1;
}

bool Socket::listen() const {
    if (!this->is_valid()) return false;

    int status = ::listen(this->_sock, MAXCONNECTIONS);

    return status != -1;
}

void* connection_handler(void* socket_desc) {
    int sock = *(int*)socket_desc;
    int read_size;
    char* message, client_message[2000];

    message = "Greetings!";
    write(sock, message, strlen(message));
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
        client_message[read_size] = '\0';
        write(sock, client_message, strlen(client_message));
        memset(client_message, 0, 2000);
    }

    if (read_size == 0) {
        std::cout << "Client Disconnected" << std::endl;
    } else if (read_size == -1) {
        std::cout << "recv failed";
    }
    close(sock);
    return 0;
}

bool Socket::accept(Socket& new_socket) const {
    int addr_length = sizeof(this->_addr);
    new_socket._sock = ::accept(this->_sock, (sockaddr*)&this->_addr, (socklen_t*)&addr_length);

    pthread_t thread;

    if (pthread_create(&thread, NULL, connection_handler, (void*)&new_socket._sock) < 0) {
        std::cout << "thread NOT created\n";
        return false;
    }
    std::cout << "thread created\n";

    return new_socket._sock > 0;
}

bool Socket::send(const std::string& message) const {
    int status = ::send(this->_sock, message.c_str(), message.size(), MSG_NOSIGNAL);

    return status != -1;
}

int Socket::recv(std::string& s) const {
    char buffer[MAXRECV + 1];
    s = "";
    memset(buffer, 0, MAXRECV + 1);

    int status = ::recv(this->_sock, buffer, MAXRECV, 0);

    if (status > 0) {
        s = buffer;
    }
    return status;
}

bool Socket::connect(const std::string& host, const int port) {
    if (!this->is_valid()) return false;

    this->_addr.sin_family = AF_INET;
    this->_addr.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &this->_addr.sin_addr);

    if (errno == EAFNOSUPPORT) return false;

    status = ::connect(this->_sock, (sockaddr*)&this->_addr, sizeof(this->_addr));

    return status == 0;

}

bool Socket::is_valid() const {
    return this->_sock != -1;
}

#endif // SOCKET_H

