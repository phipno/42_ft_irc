#include "Server.Class.hpp"
#include "Client.Class.hpp"

int main(int argc, char **argv){

    (void) argc;
	int port = std::atoi(argv[1]); 
    Server server(port, "password");
    server.runServer();

    return (0);
}

// To test server open new terminal and enter:
// nc localhost 6667#include "Client.Class.hpp"


// #include "Channel.Class.hpp"

// int main(void)
// {
// 	Channel chnl("test", false, false, "", -1);
// 	Client user1("User1", "Lee Roy");

// 	chnl.join("user1");
// 	chnl.join("user2");
// 	chnl.give_priveleges("user1");
// 	chnl.list_clients();
// 	chnl.kick("user2");
// 	chnl.list_clients();
// 	std::cout << chnl.get_topic() << std::endl;
// 	chnl.topic("Neu");
// 	std::cout << chnl.get_topic() << std::endl;
// 	chnl.list_channel_attributes();
// }