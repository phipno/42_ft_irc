#include "Server.Class.hpp"
#include "Client.Class.hpp"

int main(int argc, char **argv){

    int port = std::atoi(argv[1]); 
    Server server(port, "password");
    server.runServer();

    return (0);
}

// To test server open new terminal and enter:
// nc localhost 6667