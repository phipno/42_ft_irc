#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"

#include "defines.hpp"


std::string Server::numReply(int errorCode, t_msg *message, Client &client){

	std::string reply;

	switch(errorCode){
		case RPL_WELCOME:
			reply =  ": " + this->_hostname + " 001 " + client.getNickName() + " :Welcome to the Internet Relay Network, " + client.getNickName() + "!" +client.getUserName() + "@" + this->_hostname;
			send_msg_to_client_socket(client, reply);
			break;
		case RPL_YOURHOST:
			reply =  ": " + this->_hostname + " 002 " + client.getNickName() + " :Your host is " + this->_hostname + ", running on version 1.0 !";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_NONICKNAMEGIVEN:
			reply =  "to be specified";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_NOSUCHNICK:
			reply =  "401 - to be specified";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_ERRONEUSNICKNAME:
			reply =  "to be specified";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_NORECIPIENT:
			reply =  "to be specified - no recipient";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_NICKNAMEINUSE:
			reply =  "to be specified";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_ALREADYREGISTRED:
			reply = ": " + this->_hostname + " 462 " + client.getNickName() + " :You may not reregister\n";
			send_msg_to_client_socket(client, reply);
			break;
		case ERR_NEEDMOREPARAMS:
			reply = ": " + this->_hostname + " 461 " + client.getNickName() + " :" +message->command + ": Not enough parameters";
			send_msg_to_client_socket(client, reply);
			break;
		default:
			std::cout << "unkown error\n";
	}
	if (DEBUG)
		std::cout << reply << std::endl;
	return "NULL";
}

