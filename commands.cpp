#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"

/* --------------- Commands for all users ------------------------------------*/


/*
3.7.2 Ping message
Command: PING
Parameters: <server1> [ <server2> ]
The PING command is used to test the presence of an active client or
server at the other end of the connection.  Servers send a PING
message at regular intervals if no other activity detected coming
from a connection.  If a connection fails to respond to a PING
message within a set amount of time, that connection is closed.  A
PING message MAY be sent even if the connection is active.
When a PING message is received, the appropriate PONG message MUST be
sent as reply to <server1> (server which sent the PING message out)
as soon as possible.  If the <server2> parameter is specified, it
represents the target of the ping, and the message gets forwarded
there.
   Numeric Replies:
           ERR_NOORIGIN                  ERR_NOSUCHSERVER
   Examples:
   PING tolsun.oulu.fi             ; Command to send a PING message to
                                   server
   PING WiZ tolsun.oulu.fi         ; Command from WiZ to send a PING
                                   message to server "tolsun.oulu.fi"
   PING :irc.funet.fi              ; Ping message sent by server
                                   "irc.funet.fi"*/

//server sends a ping message to the client. Client has to respond with a pong message


int Server::handshake(t_msg *message, class Client &client) {

	// if (message->paramVec.size() >= 3)
		send_msg_to_client_socket(client, "PONG " + message->paramVec[1]);
	return (1);
}

int Server::pong(t_msg *message, class Client &client) {

	// if (message->paramVec.size() >= 2)
	std::string msg = "PONG ";
	std::vector<std::string>::iterator it = message->paramVec.begin();
	for ( ; it != message->paramVec.end(); it++)
		msg += *it + " ";
		
	send_msg_to_client_socket(client, msg);
	return (1);
}



void Server::list(t_msg &message, Client &client) {
	std::vector<Channel>::iterator it = _channels.begin();

	if (message.paramVec.empty()) {
		send_msg_to_client_socket(client, "------- LIST OF CHANNELS -------");
		for ( ; it != _channels.end(); it++) {	
			std::cout << it->get_name() << std::endl;
			send_msg_to_client_socket(client, it->get_name());
		}
		send_msg_to_client_socket(client, "------- LIST OF CHANNELS -------");
	}
}

//used for creating or joining a channel, depending if it is already existent
void Server::join_channel(std::string channelName, class Client &client) {
	
	int i = channel_exists(channelName);
	std::cout << "Channel already exists? " << i << std::endl;
	if (i == -1) {
		Channel channel(channelName);
		channel.add_user(client.getNickName(), "", true);
		this->_channels.push_back(channel);
	}
	else{
		this->_channels[i].add_user(client.getNickName(), "", false);
	}
}

int Server::channel_exists(std::string channelName) {

	std::vector<Channel>::iterator it = this->_channels.begin();
	for ( int i = 0; it != this->_channels.end(); it++, i++) {
		if (channelName == it->get_name())
			return (i);
	}
	return (-1);
}

// ERR_PING(client)
// define ERR_PING(client){"": To connect, type PONG 1234567890"}
// send_msg_to_client_socket(client, ERR_PING(client));

std::string Server::make_msg_ready(t_msg *message, Client &client, size_t channelnumber, std::string topic_message) {

	std::string msg;

	if (message->command == "PRIVMSG") {

		msg += ":" + client.getNickName() + "!~" + client.getUserName() + "@" + _hostname + \
		 " " + message->command + " " + message->paramVec[0] + " " + message->paramVec[1];
	}
	else if (message->command == "JOIN") {

		msg += ":" + client.getNickName() + "!~" + client.getUserName() + "@" + _hostname + \
		 " " + message->command + " " + message->paramVec[channelnumber];
	}
	else if (message->command == "TOPIC") {

		msg += ":" + client.getNickName() + "!~" + client.getUserName() + "@" + _hostname + \
		 " " + message->command + " " + message->paramVec[channelnumber] + " :"  + topic_message;
	}
	else if (message->command == "INVITE") {

		msg += ":" + client.getNickName() + "!~" + client.getUserName() + "@" + _hostname + \
		 " " + message->command + " " + message->paramVec[0] + " :"  + message->paramVec[1];
	}

	return (msg);
}



