#include "Command.Class.hpp"

#pragma once

class Invite : public Command{

	private:
		int i;
		Invite();		

	public:
		Invite(Server &server, Client &client, std::string message);
		virtual ~Invite();

		virtual int executeCommand();
		int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);
		void communicateInvite(std::string nick, std::string channel, Client &client);
};
