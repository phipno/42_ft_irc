#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"

#include "defines.hpp"


// std::string Server::numReply(int errorCode, t_msg *message, Client &client){
void Server::numReply(Client &client, std::string message){

	send_msg_to_client_socket(client, message);

	// switch(errorCode){


	// 	case ERR_PING:
	// 		reply = ": To connect, type PONG 1234567890";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case ERR_NOORIGIN:
	// 		reply = ": To connect, type PONG 1234567890";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;

	// 	case ERR_CHANOPRIVSNEEDED:
	// 		reply = ": You're not channel operator";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case RPL_TOPIC:
	// 		reply = ": <channel> :Topic is <TOPIC message>";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case RPL_NOTOPIC:
	// 		reply = ": <channel> :No topic is set";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	default:
	// 		std::cout << "unkown error\n";
	// }
	// if (DEBUG)
	// 	std::cout << reply << std::endl;
	// return "NULL";
}

