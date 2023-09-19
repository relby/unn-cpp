#include "server.h"
#include "../common/utils.h"

const uint16_t PORT = 58000;

int main(int argc, char** argv) {
    uint16_t port = argc >= 2 ? std::stoll(argv[1]) : PORT; 
    Server server(port);
    if (server.establish() < 0) {
        std::cerr << "[INFO]: failed to establish a listening socket, terminating" << std::endl;
        return 1;
    }

    while (true) {
        sockaddr_in client_addr;
        socklen_t slen = sizeof(client_addr);
        int client_socket_fd = accept(server.get_socket_fd(), (struct sockaddr *)&client_addr, &slen);
        if (client_socket_fd < 0) {
            server.close();
            std::cout << "[ERORR]: Can't accept a connection with client request" << std::endl;
            return 1;
        }

        std::string client_ip;
        std::string client_port;
        parse_socket(&client_addr, client_ip, client_port);

        std::cout << "[INFO]: established a connection with client: " << client_ip << " " << client_port <<std::endl;
        std::cout << "[INFO]: starting thread for client: " << client_ip << std::endl;
        std::thread t1(handle_message, &server, client_socket_fd);
        t1.detach();
    }
}
