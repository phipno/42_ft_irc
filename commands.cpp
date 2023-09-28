#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"

/* Commands for all users*/


// PASS
// first step, even before connecting with a nickname
// entry from client: PASS YourServerPassword
// variables in t_msg struct:
//      message.command = PASS
//      message.paramVec[0] = YourServerPassword

    //    462    ERR_ALREADYREGISTRED
    //           ":Unauthorized command (already registered)"

    //      - Returned by the server to any link which tries to
    //        change part of the registered details (such as
    //        password or user details from second USER message).

        //    461    ERR_NEEDMOREPARAMS
        //       "<command> :Not enough parameters"

        //  - Returned by the server by numerous commands to
        //    indicate to the client that it didn't supply enough
        //    parameters.

int Server::pass(t_msg *message, Client &client){
	if (client.getRegistrationStatus() >= REGISTERED)
		numReply(462, message, client);
	else if (message->paramVec.empty() || message->paramVec[0].empty())
		numReply(461, message, client);
	else if (message->paramVec[0].compare(this->_password) == 0){
        client.registerClient(true);
		std::cout << "Registering succesfull: Client status is " << client.getStatus() << std::endl;
		return 0;
	}
	return 1;
}

// NICK
// NICK <your-nick>
// when connecting for the first time, choose a nickname

        //    ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
        //    ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
        //    ERR_UNAVAILRESOURCE             ERR_RESTRICTED

    //    431    ERR_NONICKNAMEGIVEN
    //           ":No nickname given"

    //      - Returned when a nickname parameter expected for a
    //        command and isn't found.

    //    432    ERR_ERRONEUSNICKNAME
    //           "<nick> :Erroneous nickname"

    //      - Returned after receiving a NICK message which contains
    //        characters which do not fall in the defined set.  See
    //        section 2.3.1 for details on valid nicknames.

    //    433    ERR_NICKNAMEINUSE
    //           "<nick> :Nickname is already in use"

    //      - Returned when a NICK message is processed that results
    //        in an attempt to change to a currently existing
    //        nickname.

    //        436    ERR_NICKCOLLISION
    //           "<nick> :Nickname collision KILL from <user>@<host>"

    //      - Returned by a server to a client when it detects a
    //        nickname collision (registered of a NICK that
    //        already exists by another server).

    //    437    ERR_UNAVAILRESOURCE
    //           "<nick/channel> :Nick/channel is temporarily unavailable"

    //      - Returned by a server to a user trying to join a channel
    //        currently blocked by the channel delay mechanism.

    //      - Returned by a server to a user trying to change nickname
    //        when the desired nickname is blocked by the nick delay
    //        mechanism.

    //    484    ERR_RESTRICTED
    //       ":Your connection is restricted!"

    //      - Sent by the server to a user upon connection to indicate
    //      the restricted nature of the connection (user mode "+r").

int Server::nick(t_msg *message, Client &client){
// TODO(albert) - sending a message to the according client, wether the name exists, must be implemented
    
    if (VERBOSE)
        std::cout << "nick()" << std::endl;

	if (client.getRegistrationStatus() < REGISTERED)
		return 1; //send error message
	if (message->paramVec.empty() || message->paramVec[0].empty()){
		numReply(431, message, client);
		return 1;
	}
    // check if characters of chosen nickname are valid
    std::string allowed_chars = "abcdefghijklmnopqrstuvwxyz0123456789{}[]\\|";
	for (size_t i = 0; i < message->paramVec[0].length(); ++i){
        char ch = message->paramVec[0][i];
		if (allowed_chars.find(ch) != std::string::npos){

        }
        else{
			numReply(432, message, client);
			return 1;
        }
	}
    // check if nickname already exists on same server
	std::vector<Client>::iterator it = _clients.begin();
	for (;it != _clients.end(); it++){
		if (it->getNickName() == message->paramVec[0]){
			numReply(433, message, client);
			return 1;
        }
	}

	client.setNickName(message->paramVec[0]);
	if (client.getNickName() == "superuser") {
		numReply(001, message, client); // welcome
		numReply(002, message, client); // your host
		client.setSu(true);
		client.registerClient(SUPERUSER);
		return (0);
	}
	ping(client);
	return 0;
}

// USER <your-username> <your-hostname> <your-servername> :<your-realname>
// ex:
// USER MyUsername MyHostname MyServername :John Doe

// JOIN
// JOIN #channelname


int Server::user(t_msg *message, Client &client){

    if (VERBOSE)
        std::cout << "user()" << std::endl;

	if (client.getRegistrationStatus() >= USERNAME){
		numReply(462, message, client); //already registered
		return 1;
	}
	if (message->paramVec.empty() || message->paramVec[0].empty()){
		numReply(461, message, client); // need more params
		return 1;
	}
	if (message->paramVec[0].length() <= USERLEN)
		client.setFullName(message->paramVec[0]);
	else{
		std::string name = message->paramVec[0].substr(0,9);
		client.setFullName(name);
	}
	
	if (client.getStatus() == NICKNAME) {
		numReply(001, message, client); // welcome
		numReply(002, message, client); // your host
		client.registerClient(WELCOMED);
	}
	else 
		client.registerClient(USERNAME);
    return (0); //eventually other value
}

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
void Server::ping(class Client &client) {

		send_msg_to_client_socket(client, "PING :1234567890");
}

int Server::handshake(t_msg *message, class Client &client) {

	send_msg_to_client_socket(client, "CAP REC PONG: " + message->paramVec[2]);
	return (1);
}

// //older version
int Server::pong(t_msg *message, class Client &client) {

	// std::string pong_msg = recv_from_client_socket(client);
	if (message->paramVec[0] == "") {
		numReply(409, message, client); //ERR_NOOIRIGIN means, no parameter fpr /PONG
		return (1);
	}
	if (message->paramVec[0] == "ft_irc") {
		if (client.getStatus() == USERNAME) {
			numReply(001, message, client); // welcome
			numReply(002, message, client); // your host
			client.setStatus(WELCOMED);
		}
		else 
			client.setStatus(NICKNAME);
		return (0);
	}
	else if (message->paramVec[0] != "ft_irc")
		numReply(005, message, client);
		return (1);
}

// PRIVMSG
// PRIVMSEG <msgtarget> <text to be sent>

//    Parameters: <msgtarget> <text to be sent>

//    PRIVMSG is used to send private messages between users, as well as to
//    send messages to channels.  <msgtarget> is usually the nickname of
//    the recipient of the message, or a channel name.

//    The <msgtarget> parameter may also be a host mask (#<mask>) or server
//    mask ($<mask>).  In both cases the server will only send the PRIVMSG
//    to those who have a server or host matching the mask.  The mask MUST
//    have at least 1 (one) "." in it and no wildcards following the last
//    ".".  This requirement exists to prevent people sending messages to
//    "#*" or "$*", which would broadcast to all users.  Wildcards are the
//    '*' and '?'  characters.  This extension to the PRIVMSG command is
//    only available to operators.

//    Numeric Replies:

//            ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//            ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//            ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//            ERR_NOSUCHNICK
//            RPL_AWAY

int Server::privmsg(t_msg *message, Client &client){
	if (client.getRegistrationStatus() < USERNAME){
		numReply(462, message, client); // not sure which message to send, if user is not fully registered
		return 1;
	}
	else if (message->paramVec.empty() || message->paramVec[0].empty()){
		numReply(411, message, client); // ERR_NORECIPIENT
		return 1;
	}
	else if (message->paramVec[1].empty()){
		numReply(412, message, client); // ERR_NOTEXTTOSEND
		return 1;		
	}
	std::string msg = message->paramVec[1];
	if (!message->paramVec[0].empty())
	{
		if (message->paramVec[0].at(0) == '#'){
			int i = channel_exists(message->paramVec[0]);
			if (i != -1)
				send_message_to_channel(message->paramVec[1], this->_channels[i]);
			else
				numReply(403, message, client); // ERR_ NOSUCHCHANNEL
		}
		else {
			std::vector<Client>::iterator clientit = _clients.begin();
			for (; clientit < _clients.end(); clientit++){
				if (clientit->getNickName() == message->paramVec[0]){
					send_msg_to_client_socket(*clientit, msg);
					break;	
				}
				numReply(412, message, client); // ERR_NOSUCHNICK			
			}
		}
	}
	return (0);
}

// 001    RPL_WELCOME
//               "Welcome to the Internet Relay Network
//                <nick>!<user>@<host>"
//        002    RPL_YOURHOST
//               "Your host is <servername>, running version <ver>"
//        003    RPL_CREATED
//               "This server was created <date>"
//        004    RPL_MYINFO
//               "<servername> <version> <available user modes>
//                <available channel modes>"

//          - The server sends Replies 001 to 004 to a user upon
//            successful registration.

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
		channel.add_user(client.getNickName(), true);
		this->_channels.push_back(channel);
	}
	else {
		this->_channels[i].add_user(client.getNickName(), false);
	}
}

void Server::join(t_msg &parsedMsg, Client &client) {
	(void)parsedMsg;
	(void)client;
}

int Server::channel_exists(std::string channelName) {

	std::vector<Channel>::iterator it = this->_channels.begin();
	for ( int i = 0; it != this->_channels.end(); it++, i++) {
		if (channelName == it->get_name())
			return (i);
	}
	return (-1);
}

/*Command: TOPIC
Parameters: <channel> [ <topic> ]
The TOPIC command is used to change or view the topic of a channel.
The topic for channel <channel> is returned if there is no <topic>
given.  If the <topic> parameter is present, the topic for that
channel will be changed, if this action is allowed for the user
requesting it.  If the <topic> parameter is an empty string, the
topic for that channel will be removed.
   Numeric Replies:
           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES
   Examples:
   :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
                                   topic.
   TOPIC #test :another topic      ; Command to set the topic on #test
                                   to "another topic".
   TOPIC #test :                   ; Command to clear the topic on
                                   #test.
   TOPIC #test                     ; Command to check the topic for
                                   #test.
*/
int Server::topic(t_msg *parsedMsg, Client &client) {
	
	int i = channel_exists(parsedMsg->paramVec[0]);
	//if channel does not exist, return USER not on channel
	if (i == -1) { 
		numReply(ERR_NOTONCHANNEL, parsedMsg, client);
		return (1);
	}

	//checks if the client is on that channel
	if (!_channels[i].is_in_channel(client.getNickName())) {
		numReply(ERR_NOTONCHANNEL, parsedMsg, client);
		return (1);
	}

	// if no topic argument exists, the Topic will be displayed
	// if there is an empty string the Topic will be deleted (operator)
	// else the topic will be set if there is a non-empty string (operator)	
	bool privileges = _channels[i].is_operator(client.getNickName());
	std::vector<std::string>::iterator it = parsedMsg->paramVec.begin() + 1;

	if (it == parsedMsg->paramVec.end())
		numReply(RPL_TOPIC, parsedMsg, client);
	else if (is_empty_string(*(++it)) && (privileges || !_channels[i].get_topic_restriction()))
		_channels[i].set_topic(" :No topic set");
	else if (is_empty_string(*(++it)) && _channels[i].get_topic_restriction() && !privileges)
		numReply(ERR_CHANOPRIVSNEEDED, parsedMsg, client);
	else if (!is_empty_string(*(++it)) && (privileges || !_channels[i].get_topic_restriction()))
		_channels[i].set_topic(*it);
	else if (!is_empty_string(*(++it)) && _channels[i].get_topic_restriction() && !privileges)
		numReply(ERR_CHANOPRIVSNEEDED, parsedMsg, client);
	return (0);
}


// ERR_PING(client)
// define ERR_PING(client){"": To connect, type PONG 1234567890"}
// send_msg_to_client_socket(client, ERR_PING(client));





