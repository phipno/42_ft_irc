#include "Server.Class.hpp"

int main(){

    Server server(6667, "password");
    server.setup();

    return (0);
}