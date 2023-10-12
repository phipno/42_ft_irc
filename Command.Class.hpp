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
		std::string					_prefix;
		std::string					_command;
		std::string					_param;
		std::vector<std::string>	_paramVec;
		Client						*_client;
		Server						*_server;
		std::vector<Channel>		*_channels;

	public:
		Command(Server &server, Client &client, std::string message);
		Command &operator=(Command const &src);
		virtual ~Command();

		void tokenizeMsg();
		int numReply(std::string message);
		std::string makeMsgReady(Client &client, size_t channelnumber, std::string topic_message);
		int checkRegistrationStatus();
		int checkRegistrationStatusWelcomed();
		int checkEmptyParamter();
		int isValidNickname();
		std::vector<std::string> parse_join_kick(std::string commaToken);

		virtual int executeCommand() = 0;
};