#include "Client.hpp"
#include "Channel.hpp"

int main(void)
{
	Channel chnl("test", false, false, "", -1);
	Client user1("User1", "Lee Roy");

	chnl.join("user1");
	chnl.join("user2");
	chnl.give_priveleges("user1");
	chnl.list_clients();
	chnl.kick("user2");
	chnl.list_clients();
	std::cout << chnl.get_topic() << std::endl;
	chnl.topic("Neu");
	std::cout << chnl.get_topic() << std::endl;
	chnl.list_channel_attributes();
}