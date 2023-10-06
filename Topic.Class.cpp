#include "Topic.Class.hpp"

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

Topic::Topic(Server &server, Client &client, std::string message) : Command(server, client , message){}

Topic::~Topic() {};

/* ---------------------- METHODS --------------------------------------------*/


int Topic::executeCommand(){

	Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;

	int i = channel_exists(this->_paramVec[0]);
	if (i == -1) { 
		numReply(client, ERR_NOTONCHANNEL(this->_hostname, client.getNickName(), parsedMsg->paramVec[0]));
		return (1);
	}

	//checks if the client is on that channel
	if (!_channels[i].is_in_channel(client.getNickName())) {
		numReply(client, ERR_NOTONCHANNEL(this->_hostname, client.getNickName(), _channels[i].get_name()));
		return (1);
	}

	// if no topic argument exists, the Topic will be displayed
	// if there is an empty string the Topic will be deleted (operator)
	// else the topic will be set if there is a non-empty string (operator)	
	bool privileges = _channels[i].is_operator(client.getNickName());
	std::vector<std::string>::iterator it = parsedMsg->paramVec.begin() + 1;

	// TO-DO: make topic available for multiple channels
	//e.g: TOPIC #chan1,#chan2 :new_topic changes the topic for both channels into new_topic
	//loop through the code below
	if (it == parsedMsg->paramVec.end()) {
		// numReply(client, RPL_TOPIC(this->_hostname, client.getNickName(), _channels[i].get_name(), _channels[i].get_topic()));
		return(0);
	}
	if (is_empty_string(*it) && (privileges || !_channels[i].get_topic_restriction()))
		_channels[i].set_topic("No topic is set");
	else if (is_empty_string(*it) && _channels[i].get_topic_restriction() && !privileges)
		numReply(client, ERR_CHANOPRIVSNEEDED(this->_hostname, client.getNickName(), _channels[i].get_name()));
	else if (!is_empty_string(*it) && (privileges || !_channels[i].get_topic_restriction())) {
		_channels[i].set_topic(*it);
		std::string msg = make_msg_ready(parsedMsg, client, 0, _channels[i].get_topic());
		send_message_to_channel(msg, _channels[i]);
	}
	else if (!is_empty_string(*it) && _channels[i].get_topic_restriction() && !privileges)
		numReply(client, ERR_CHANOPRIVSNEEDED(this->_hostname, client.getNickName(), _channels[i].get_name()));
	return (0);
}