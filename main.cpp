#include "Server.Class.hpp"
#include "Client.Class.hpp"

int main(){

    Server server(6667, "password");
    server.runServer();

    return (0);
}

// To test server open new terminal and enter:
// nc localhost 6667