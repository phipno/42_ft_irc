#include "Server.Class.hpp"
#include "Channel.Class.hpp"
#include "Client.Class.hpp"

extern bool g_sigint;
void signal_handler(int binary) {

        std::cout << "Binary: " << binary << std::endl;
    if (binary == SIGINT) {
        std::cout << "Closing Server and disconnecting clients" << std::endl;
        g_sigint = true;
    }
}

//server
int main(int argc, char **argv) {

    (void) argc;
    if (argc == 3) {
        int port = std::atoi(argv[1]); 
        Server server(port, argv[2]);
        signal(SIGINT, signal_handler);
        while(!g_sigint) {

        std::cout << "main()" << g_sigint << std::endl;
            server.runServer();
        }
        for (unsigned int i = 0; i < server.get_clients().size(); i++) {
            std::cout << "Closing Client Socket: " << close(server.get_clients()[i].getClientSocket());
        }
        std::cout << "Closing Server Socket: " << close(server.get_serversocket()) << std::endl;
    }
    else {
        std::cerr << "Error: Usage ./ircserv <PORT> <PASSWORD>" << std::endl;
        return (1);
    }
    return (0);
}
