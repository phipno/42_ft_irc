#include "Server.Class.hpp"
#include "Client.Class.hpp"

int main(){

    Server server(6667, "password");
    server.setup();

    return (0);
}

// To test server open new terminal and enter:
// nc localhost 6667