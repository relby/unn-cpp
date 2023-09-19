#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "common.h"
#include "client_socket.h"

const std::string HOST = "localhost";
const uint16_t PORT = 8888;

int main() {
    bool quit = false;
    try {
        ClientSocket client_socket(HOST, PORT);
        std::string message, reply;
        try {
            while (!quit) {
                std::cout << "[CLIENT]: ";
                std::getline(std::cin, message);
                client_socket << message;

                client_socket >> reply;
                std::cout << "[SERVER]: " << reply << '\n';

                if (message == QUIT_COMMAND) {
                    quit = true;
                }
            }
        } catch (const SocketException&) {}
    } catch (const SocketException& e) {
        std::cout << "Exception was caught: " << e.message() << '\n';
    }
}
