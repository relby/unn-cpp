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

#include "../common/socket.h"
#include "../common/utils.h"
#include "../common/commands.h"
#include "server_exception.h"
#include "patient_repository.h"

const size_t MAX_RECV = 1024;

class Server : public Socket {
private:
    PatientRepository& _patients;
public:
    std::string handle_command(const std::string& msg, bool& quit) const;
    Server(const uint16_t port);
    int establish();
    int send(int sockfd, const std::string &msg) override;
    int recv(int sockfd, std::string &msg) override;
};

std::mutex mtx;
void handle_message(Server* server, int client_sock_fd);

Server::Server(const uint16_t port): Socket(), _patients(PatientRepository::instance()) {
    std::cout << "[INFO]: Initializing a server instance ..." << std::endl;
    std::cout << "[INFO]: Collecting server information ..." << std::endl;
    memset(&this->_addr, 0, sizeof(this->_addr));
    gethostname(this->_hostname, sizeof(this->_hostname));

    hostent *hp;
    if ((hp = gethostbyname(this->_hostname)) == NULL) {
        throw std::invalid_argument("[ERROR]: Can't get host info");
    }

    this->_addr.sin_family = hp->h_addrtype;
    this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this->_addr.sin_port = htons(port);

    std::string server_host;
    std::string server_port;

    parse_socket(&this->_addr, server_host, server_port);
    std::cout << "[INFO]: Host " << server_host << " Port " << server_port << std::endl;
}

int Server::establish() {
    if ((this->_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "[ERORR]: Failed to create a socket" << std::endl;
        return -1;
    }

    std::cout << "[INFO]: Open a socket successfully" << std::endl;
    if (bind(this->_socket, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0) {
        this->close();
        std::cout << "[ERORR]: Failed to bind address with socket" << std::endl;
        return -1;
    }
    std::cout << "[INFO]: Bind address to socket successfully" << std::endl;
    if ((listen(this->_socket, 3)) < 0) {
        this->close();
        std::cout << "[ERORR]: Failed to listen the socket" << std::endl;
        return -1;
    }
    std::cout << "[INFO]: Waiting for a connection request from clients" << std::endl;
    return 0;
}

int Server::recv(int client_sock_fd, std::string &msg) {
    char buffer[MAX_RECV + 1];
    bool quit = false;
    while (!quit) {
        memset(buffer, 0, sizeof(buffer));
        int state = ::recv(client_sock_fd, buffer, MAX_RECV, 0);
        if (state < 0) {
            std::cout << "[ERORR]: Failed to receive the echo message to client, closing socket" << std::endl;
            ::close(client_sock_fd);
            return -1;
        } else if (state == 0) {
            std::cout << "[INFO]: Detected client closed, closing socket ..." << std::endl;
            ::close(client_sock_fd);
            return -1;
        } else if (state < MAX_RECV) {
            quit = true;
        }
        msg.append(std::string(buffer));
    }
    return 0;
}

int Server::send(int client_sock_fd, const std::string &msg) {
    uint32_t total_sent = 0;
    uint32_t sent = 0;
    while (total_sent < msg.size()) {
        if ((sent = ::send(client_sock_fd, msg.c_str() + total_sent, msg.size() - total_sent, 0)) < 0) {
            std::cout << "[ERROR]: Failed to send the echo message to client, closing socket ...";
            ::close(client_sock_fd);
            return  -1;
        }
        total_sent += sent;
    }
    return 0;
}

void handle_message(Server* server, int client_sock_fd) {
    std::string message, reply;
    bool quit = false;
    while (!quit) {
        message.clear();
        if (server->recv(client_sock_fd, message) < 0) break;
        mtx.lock();
        reply = server->handle_command(message, quit);
        mtx.unlock();
        if (server->send(client_sock_fd, reply) < 0) break;
    }
}

std::string Server::handle_command(const std::string& message, bool& quit) const {
    if (message == QUIT_COMMAND) {
        quit = true;
        return "Bye bye";
    } else if (message == LIST_COMMAND) {
        auto patients = this->_patients.all_patients();
        std::vector<std::string> reply_vector = { "Patients: " };
        for (size_t i = 0; i < patients.size(); i++) {
            auto patient = patients[i];
            reply_vector.push_back("  " + std::to_string(i + 1) + ") " + patient->to_string());
        }
        return join(reply_vector, "\n");
    } else if (message == GET_COMMAND) {
        try {
            return this->_patients.get_patient()->to_string();
        } catch (const std::out_of_range&) {
            return "Patient queue is empty";
        }
    } else if (message.starts_with(ADD_COMMAND)) {
        auto params = split(message, " ");
        if (params.size() < 3) {
            return "Not enough arguments";
        }
        if (params.size() == 3) {
            try {
                this->_patients.add_patient(
                    make_shared<Patient>(
                        params[1],
                        string_to_patient_state(params[2])
                    )
                );
                return "Patient successfully added to queue";
            } catch (const parse_error& e) {
                return e.what();
            } catch (const std::invalid_argument& e) {
                return e.what();
            }
        }
        if (params.size() == 4) {
            try {
                this->_patients.add_patient(
                    make_shared<VipPatient>(
                        params[1],
                        string_to_patient_state(params[2]),
                        std::stod(params[3])
                    )
                );
                return "VipPatient successfully added to queue";
            } catch (const parse_error& e) {
                return e.what();
            } catch (const std::invalid_argument& e) {
                return "Could not parse money parameter";
            }
        } else {
            return "Too many arguments";
        }
    } else {
        return "There is no such a command";
    }
}
