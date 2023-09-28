#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"

/* --------------- Commands for all users ------------------------------------*/


/* PASS
first step, even before connecting with a nickname
entry from client: PASS <YourServerPassword>
variables in t_msg struct:
	message.command = PASS
	message.paramVec[0] = YourServerPassword

	462	ERR_ALREADYREGISTRED
	461	ERR_NEEDMOREPARAMS
*/

int Server::pass(t_msg *message, Client &client){
	if (client.getRegistrationStatus() >= REGISTERED)
		numReply(client, ERR_ALREADYREGISTERED(this->_hostname, client.getNickName()));
	else if (message->paramVec.empty() || message->paramVec[0].empty())
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), message->command));
	else if (message->paramVec[0].compare(this->_password) == 0){
		client.registerClient(true);
		std::cout << "Registering succesfull: Client status is " << client.getStatus() << std::endl;
		return 0;
	}
	return 1;
}

/* NICK
NICK <your-nick>
when connecting for the first time, choose a nickname

	ERR_NONICKNAMEGIVEN		ERR_ERRONEUSNICKNAME
	ERR_NICKNAMEINUSE		ERR_NICKCOLLISION
	ERR_UNAVAILRESOURCE		ERR_RESTRICTED
*/

int Server::nick(t_msg *message, Client &client){
// TODO(albert) - sending a message to the according client, wether the name exists, must be implemented
    
    if (VERBOSE)
        std::cout << "nick()" << std::endl;

	if (client.getRegistrationStatus() < REGISTERED)
		return 1; //send error message?
	if (message->paramVec.empty() || message->paramVec[0].empty()){
			numReply(client, ERR_NONICKNAMEGIVEN(this->_hostname));
		return 1;
	}
    // check if characters of chosen nickname are valid
    std::string allowed_chars = "abcdefghijklmnopqrstuvwxyz0123456789{}[]\\|";
	for (size_t i = 0; i < message->paramVec[0].length(); ++i){
        char ch = message->paramVec[0][i];
		if (allowed_chars.find(ch) != std::string::npos){}
        else{
			numReply(client, ERR_ERRONEUSNICKNAME(this->_hostname, client.getNickName()));
			return 1;
        }
	}
    // check if nickname already exists on same server
	std::vector<Client>::iterator it = _clients.begin();
	for (;it != _clients.end(); it++){
		if (it->getNickName() == message->paramVec[0]){
			numReply(client, ERR_NICKNAMEINUSE(this->_hostname, client.getNickName()));
			return 1;
        }
	}

	client.setNickName(message->paramVec[0]);
	if (client.getNickName() == "superuser") {
		numReply(client, RPL_WELCOME(this->_hostname, client.getNickName(), client.getUserName()));
		numReply(client, RPL_YOURHOST(this->_hostname, client.getNickName()));
		client.setSu(true);
		client.registerClient(SUPERUSER);
		return (0);
	}
	ping(client);
	return 0;
}

/* USER <your-username> <your-hostname> <your-servername> :<your-realname>
*/

int Server::user(t_msg *message, Client &client){

    if (VERBOSE)
        std::cout << "user()" << std::endl;

	if (client.getRegistrationStatus() >= USERNAME){
		numReply(client, ERR_ALREADYREGISTERED(this->_hostname, client.getNickName()));
		return 1;
	}
	if (message->paramVec.empty() || message->paramVec[0].empty()){
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), message->command));
		return 1;
	}
	if (message->paramVec[0].length() <= USERLEN)
		client.setFullName(message->paramVec[0]);
	else{
		std::string name = message->paramVec[0].substr(0,9);
		client.setFullName(name);
	}
	
	if (client.getStatus() == NICKNAME) {
		numReply(client, RPL_WELCOME(this->_hostname, client.getNickName(), client.getUserName()));
		numReply(client, RPL_YOURHOST(this->_hostname, client.getNickName()));
		client.registerClient(WELCOMED);
	}
	else 
		client.registerClient(USERNAME);
	numReply(client, RPL_WELCOME(this->_hostname, client.getNickName(), client.getUserName())); // rethink logic
    return 0;
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

	send_msg_to_client_socket(client, "PONG: " + message->paramVec[2]);
	return (1);
}

// //older version
int Server::pong(t_msg *message, class Client &client) {

	// std::string pong_msg = recv_from_client_socket(client);
	if (message->paramVec[0] == "") {
		// numReply(409, message, client); //ERR_NOOIRIGIN means, no parameter fpr /PONG
		return (1);
	}
	if (message->paramVec[0] == "ft_irc") {
		if (client.getStatus() == USERNAME) {
			numReply(client, RPL_WELCOME(this->_hostname, client.getNickName(), client.getUserName()));
			numReply(client, RPL_YOURHOST(this->_hostname, client.getNickName()));
			client.setStatus(WELCOMED);
		}
		else 
			client.setStatus(NICKNAME);
		return (0);
	}
	else if (message->paramVec[0] != "ft_irc")
		numReply(client, message->paramVec[0]);
		return (1);
	return (0);
}

/* PRIVMSG
PRIVMSEG <msgtarget> <text to be sent>
PRIVMSG is used to send private messages between users, as well as to
send messages to channels.  <msgtarget> is usually the nickname of
the recipient of the message, or a channel name.

The <msgtarget> parameter may also be a host mask (#<mask>) or server
mask ($<mask>).  In both cases the server will only send the PRIVMSG
to those who have a server or host matching the mask.  The mask MUST
have at least 1 (one) "." in it and no wildcards following the last
".".  This requirement exists to prevent people sending messages to
"#*" or "$*", which would broadcast to all users.  Wildcards are the
'*' and '?'  characters.  This extension to the PRIVMSG command is
only available to operators.

Numeric Replies:

	ERR_NORECIPIENT			ERR_NOTEXTTOSEND
	ERR_CANNOTSENDTOCHAN	ERR_NOTOPLEVEL
	ERR_WILDTOPLEVEL		ERR_TOOMANYTARGETS
	ERR_NOSUCHNICK
	RPL_AWAY
*/

int Server::privmsg(t_msg *message, Client &client){
	if (client.getRegistrationStatus() < USERNAME){
		return 1;
	}
	else if (message->paramVec.empty() || message->paramVec[0].empty()){
		numReply(client, ERR_NORECIPIENT(this->_hostname, client.getNickName(), message->command));
		return 1;
	}
	else if (message->paramVec[1].empty()){
		numReply(client, ERR_NOTEXTTOSEND(this->_hostname, client.getNickName()));
		return 1;		
	}
	std::string msg = message->paramVec[1];

	if (!message->paramVec[0].empty())
	{
		if (message->paramVec[0].at(0) == '#'){
			int i = channel_exists(message->paramVec[0]);
			if (i == -1)
				numReply(client, ERR_NOSUCHNICK(this->_hostname, client.getNickName()));	
			else {
			std::string channelName = message->paramVec[0];
			
			std::vector<Channel>::iterator it = this->_channels.begin();
			for (; it < _channels.end(); it++){
				if (it->get_name() == channelName && it->is_in_channel(client.getNickName())){
					send_message_to_channel(message->paramVec[1], *it);
				}
			}
			numReply(client, ERR_CANNOTSENDTOCHAN(this->_hostname, client.getNickName(), it->get_name()));
			}
		}
		else {
			std::vector<Client>::iterator clientit = _clients.begin();
			for (; clientit < _clients.end(); clientit++){
				if (clientit->getNickName() == message->paramVec[0]){
					send_msg_to_client_socket(*clientit, msg);
					break;	
				}
				numReply(client, ERR_NOSUCHNICK(this->_hostname, client.getNickName()));			
			}
		}
	}
	return (0);
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

std::vector<std::string> parse_join_kick(std::string commaToken) {
	std::vector<std::string>	splitToken;
	size_t	pos;

	while ((pos = commaToken.find(',')) != std::string::npos) {
		splitToken.push_back(commaToken.substr(0, pos));
		commaToken.erase(0, pos + 1);
	}
	if (pos == std::string::npos)
		splitToken.push_back(commaToken);

	return splitToken;
}

void Server::join(t_msg &parsedMsg, Client &client) {
	if (parsedMsg.paramVec.empty()) {
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), parsedMsg.command));
		return ;
	}
	if (parsedMsg.paramVec[0].at(0) == 0) {
		//TODO kick client out of any channel
		return;
	}
	std::vector<std::string> channelsToJoin, keyForChannel;
	std::vector<Channel> joinChannel;

	channelsToJoin = parse_join_kick(parsedMsg.paramVec[0]);
	if (parsedMsg.paramVec.size() >= 2)
		keyForChannel = parse_join_kick(parsedMsg.paramVec[1]);

	for (size_t j = 0; j < channelsToJoin.size(); j++) {
		int i = channel_exists(channelsToJoin[j]);
		if (i == -1) {
			Channel channel(channelsToJoin[j]);
			if (j < keyForChannel.size() && keyForChannel[j] != "") {
				channel.set_passphrase(keyForChannel[j]);
				channel.add_user(client.getNickName(), keyForChannel[j], true); 
				//TODO channel mode needs to be adjusted 
			}
			else
				channel.add_user(client.getNickName(), "", true); 
			//TODO add_user needs also numericReply
			this->_channels.push_back(channel);
		} else {
			if (j < keyForChannel.size() && keyForChannel[j] != "")
				this->_channels[i].add_user(client.getNickName(), keyForChannel[j], false);
			else
				this->_channels[i].add_user(client.getNickName(), "", false);
		} 
	}
	if (DEBUG) {
		for (std::vector<std::string>::iterator It = channelsToJoin.begin();
				 It != channelsToJoin.end(); ++It) {
			std::cout << "Channel: " << *It << std::endl;
		}
		for (std::vector<std::string>::iterator It = keyForChannel.begin();
				 It != keyForChannel.end(); ++It) {
			std::cout << "Keys: " << *It << std::endl;
		}
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

void Server::kick(t_msg &parsedMsg, Client &client) {
	if (parsedMsg.paramVec.empty()) {
		numReply(461, &parsedMsg, client);
		return ;
	}
	
	std::vector<std::string> channelsToKick, userToKick;
	std::vector<Channel> joinChannel;

	channelsToKick = parse_join_kick(parsedMsg.paramVec[0]);
	if (parsedMsg.paramVec.size() >= 2)
		userToKick = parse_join_kick(parsedMsg.paramVec[1]);

	for (size_t j = 0; j < channelsToKick.size(); j++) {
		int i = channel_exists(channelsToKick[j]);
		if (i == -1)
				; //TODO numReply channel doesnt exist ERR_NOSUCHCHANNEL
		// ? is this client.getNickName good, or what NAME is saved inside the channels 
		else if (this->_channels[i].is_in_channel(client.getNickName()) == false)
			;//TODO error kicker is not in channel
		else if (this->_channels[i].has_permission(client.getNickName()) == false)
			;//TODO error has no permission
		else if (channelsToKick.size() == 1) {
			std::vector<std::string>::iterator it = userToKick.begin();
			for (; it != userToKick.end(); ++it) {
				if (this->_channels[i].is_in_channel(*it) == false)
					;//TODO error user to kick is not in channel
				else {
					if (parsedMsg.paramVec.size() >= 3) {
						this->_channels[i].kick_user(userToKick[j]);
						send_msg_to_client_socket(client, "You were kicked out of #" + this->_channels[i].get_name() + "\nReason: " + parsedMsg.paramVec[2]);
					} else {
						this->_channels[i].kick_user(userToKick[j]);
						send_msg_to_client_socket(client, "You were kicked out of #" + this->_channels[i].get_name() + "\nNo Reason given");
			}
				}
			}
		} else if (j > userToKick.size()){
			if (parsedMsg.paramVec.size() >= 3) {
				this->_channels[i].kick_user(userToKick[j]);
				send_msg_to_client_socket(client, "You were kicked out of #" + this->_channels[i].get_name() + "\nReason: " + parsedMsg.paramVec[2]);
			}
			else {
				this->_channels[i].kick_user(userToKick[j]);
				send_msg_to_client_socket(client, "You were kicked out of #" + this->_channels[i].get_name() + "\nNo Reason given");
			}
		} else
			; //TODO error missing params
	}
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
		// numReply(ERR_NOTONCHANNEL, parsedMsg, client);
		return (1);
	}

	//checks if the client is on that channel
	if (!_channels[i].is_in_channel(client.getNickName())) {
		// numReply(ERR_NOTONCHANNEL, parsedMsg, client);
		return (1);
	}

	// if no topic argument exists, the Topic will be displayed
	// if there is an empty string the Topic will be deleted (operator)
	// else the topic will be set if there is a non-empty string (operator)	
	bool privileges = _channels[i].is_operator(client.getNickName());
	std::vector<std::string>::iterator it = parsedMsg->paramVec.begin() + 1;

	if (it == parsedMsg->paramVec.end()){}
		// numReply(RPL_TOPIC, parsedMsg, client);
	else if (is_empty_string(*(++it)) && (privileges || !_channels[i].get_topic_restriction()))
		_channels[i].set_topic(" :No topic set");
	else if (is_empty_string(*(++it)) && _channels[i].get_topic_restriction() && !privileges){}
		// numReply(ERR_CHANOPRIVSNEEDED, parsedMsg, client);
	else if (!is_empty_string(*(++it)) && (privileges || !_channels[i].get_topic_restriction()))
		_channels[i].set_topic(*it);
	else if (!is_empty_string(*(++it)) && _channels[i].get_topic_restriction() && !privileges){}
		// numReply(ERR_CHANOPRIVSNEEDED, parsedMsg, client);
	return (0);
}


// ERR_PING(client)
// define ERR_PING(client){"": To connect, type PONG 1234567890"}
// send_msg_to_client_socket(client, ERR_PING(client));





