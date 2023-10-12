#include "PrivMsg.Class.hpp"

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

PrivMsg::PrivMsg(Server &server, Client &client, std::string message) : Command(server, client , message){}

PrivMsg::~PrivMsg() {};

/* ---------------------- METHODS --------------------------------------------*/


int PrivMsg::executeCommand(){
    Command::tokenizeMsg();
	if (checkRegistrationStatusWelcomed())
		return 1;
	if (checkEmptyParamter())
		return 1;
    if (this->_paramVec.size() == 1){
        Command::numReply(ERR_NOTEXTTOSEND(this->_server->getHostname(), this->_client->getNickName()));
		return 1;		
	}

	std::string recipient = this->_paramVec[0];
	//make MESSAGE comply with KVirc
	std::string msg = make_msg_ready(0, "");
	// std::cout << "MESSAGE: " << msg <<std::endl;

	// if a channel
	if (recipient.at(0) == '#') {
		int i = this->_server->channel_exists(recipient);
		if (i == -1)
			Command::numReply(ERR_NOSUCHNICK(this->_server->getHostname(), this->_client->getNickName()));
		else {
			std::vector<Channel> *channel = this->_server->getChannels();
			std::vector<Channel>::iterator it = channel->begin();
			for (; it < channel->end(); it++){
				if (it->get_name() == recipient) {
					if (it->is_in_channel(this->_client->getNickName()) != false)
						this->_server->send_message_to_channel(msg, *it);
					break ;
				}
			}
			if (it == channel->end())
				Command::numReply(ERR_CANNOTSENDTOCHAN(this->_server->getHostname(), this->_client->getNickName(), ""));
		}
	}
	else {
		std::vector<Client> client = this->_server->getClients();
		std::vector<Client>::iterator clientit = client.begin();
		for ( ; clientit < client.end(); clientit++){
			if (clientit->getNickName() == recipient){
				this->_server->send_msg_to_client_socket(*clientit, msg);
				break;
			}
		}
		if (clientit == client.end())
			Command::numReply(ERR_NOSUCHNICK(this->_server->getHostname(), this->_client->getNickName()));				
	}
	return (0);
}

int PrivMsg::checkEmptyParamter(){
	if (this->_paramVec.empty()){
		Command::numReply(ERR_NORECIPIENT(this->_server->getHostname(), this->_client->getNickName(), this->_command));
		return (1);
	}
	return (0);
}

std::string PrivMsg::make_msg_ready(size_t channelnumber, std::string topic_message){
    std::string msg;
	(void) channelnumber;
	(void) topic_message;
    msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" " + this->_command + " " + this->_paramVec[0] + " " + this->_paramVec[1];
    return (msg);
}