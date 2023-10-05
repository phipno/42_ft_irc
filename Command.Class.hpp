#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"
#include "defines.hpp"

#pragma once

class Command{

	protected:
		Command();
		Command (Command const &src);
		std::string					_message;
		std::string					_prefix; //dont know if i need that
		std::string					_command;
		std::string					_param; //this could be better formulated
		std::vector<std::string>	_paramVec;
		Client						*_client;
		Server						*_server;

	public:
		Command(Server &server, Client &client, std::string message);
		Command &operator=(Command const &src);
		virtual ~Command();

		void tokenizeMsg();
		std::string returnMsgToServer(std::string message);
		std::string makeMsgReady(Client &client, size_t channelnumber, std::string topic_message);
		int checkRegistrationStatus();
		int checkEmptyParamter();

		virtual int executeCommand() = 0;
};