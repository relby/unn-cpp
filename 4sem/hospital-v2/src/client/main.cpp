#include "client.h"
#include "../common/commands.h"

const uint16_t PORT = 58000;

int main(int argc, char *argv[]) {
    uint16_t port = argc >= 2 ? std::stoll(argv[1]) : PORT; 
    Client client(port);
    if (client.create_connection() < 0) {
        std::cerr << "[ERROR]: terminating ..." << std::endl;
        return 1;
    }
    std::cout << "[INFO]: Connected with host successfully ..." << std::endl;
    std::cout << "[CLIENT]: ";

    std::string msg;
    while (std::getline(std::cin, msg)) {
        if (msg == QUIT_COMMAND) {
            std::cout << "[INFO]: Closing client socket" << std::endl;
            return 0;
        }
        if (client.send(client.get_socket_fd(), msg) < 0) {
            std::cout << "[ERROR]: failed to send the message to server, closing socket" << std::endl;
            return 1;
        }
        msg.clear();
        if (client.recv(client.get_socket_fd(), msg) < 0 ) {
            return 1;
        }
        std::cout << "[CLIENT]: ";
    }
    return 0;
}
