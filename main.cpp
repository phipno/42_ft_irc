#include "Client.hpp"
#include "Channel.hpp"

int main(void)
{
	Channel chnl("test", false, false, "", -1);
	Client user1("User1", "Lee Roy");

	chnl.join(user1);
	
}