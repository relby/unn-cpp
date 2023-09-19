#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"
#include "socket_exception.h"
#include "patient_repository.h"

class ServerSocket : private Socket {
private:
    PatientRepository& _patients;
public:
    ServerSocket(const int port);
    ServerSocket();

    const ServerSocket& operator<<(const std::string& message) const;
    const ServerSocket& operator>>(std::string& message) const;

    void accept(ServerSocket& socket);
    std::string handle_command(const std::string& command, bool& quit);
};

ServerSocket::ServerSocket(const int port)
    : Socket()
    , _patients(PatientRepository::instance())
{
    if (!Socket::create()) {
        throw SocketException("Could not create server socket");
    }
    if (!Socket::bind(port)) {
        throw SocketException("Could not bind to port");
    }
    if (!Socket::listen()) {
        throw SocketException("Could not listen to socket");
    }
}

ServerSocket::ServerSocket()
    : Socket()
    , _patients(PatientRepository::instance())
{}

const ServerSocket& ServerSocket::operator<<(const std::string& message) const {
    if (!Socket::send(message)) {
        throw SocketException("Could not write to socket");
    }
    return *this;
}

const ServerSocket& ServerSocket::operator>>(std::string& message) const {
    if (!Socket::recv(message)) {
        throw SocketException("Could not read from socket");
    }
    return *this;
}

void ServerSocket::accept(ServerSocket& sock) {
    if (!Socket::accept(sock)) {
        throw SocketException("Could not accept socket");
    }
}
std::string ServerSocket::handle_command(const std::string& command, bool& quit) {
    if (command == QUIT_COMMAND) {
        quit = true;
        return "Bye bye";
    } else if (command == LIST_COMMAND) {
        auto patients = this->_patients.all_patients();
        std::vector<std::string> reply_vector;
        for (size_t i = 0; i < patients.size(); i++) {
            auto patient = patients[i];
            reply_vector.push_back("  " + std::to_string(i + 1) + ") " + patient->to_string());
        }
        return "Patients:\n" + join(reply_vector, "\n");
    } else if (command == GET_COMMAND) {
        try {
            return this->_patients.get_patient()->to_string();
        } catch (const std::out_of_range&) {
            return "Patient queue is empty";
        }
    } else if (command.starts_with(ADD_COMMAND)) {
        auto params = split(command, " ");
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

        return "There is no such command";
    }
}

#endif // SERVER_SOCKET_H
