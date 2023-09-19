#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "common.h"
#include "server_socket.h"
#include "patient_repository.h"

const uint16_t PORT = 8888;

int main() {
    try {
        ServerSocket server(PORT);

        while (true) {
            bool quit = false;
            ServerSocket new_sock;
            server.accept(new_sock);
            std::cout << "[INFO]: Client connected\n";
            try {
                while(!quit) {
                    std::string message, reply;
                    new_sock >> message;
                    std::cout << "[CLIENT]: " << message << '\n';

                    reply = server.handle_command(message, quit);

                    new_sock << reply;
                    std::cout << "[SERVER]: " << reply << '\n';
                }
            } catch (const SocketException&) {}
            std::cout << "[INFO]: Client disconnected\n";
        }
    } catch (const SocketException& e) {
        std::cout << e.message() << '\n';
    }
}
