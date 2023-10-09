#include "Join.Class.hpp"

//TO-DO: Send channelmessge:
	//:nick1!~user1@188.244.102.158 JOIN :#test^M$ --->nick1 joined the channel (channelmessage)
//additionally send a memberlist to that client only
	// :London.UK.EU.StarLink.Org 353 nick3 * #channel2 :nick3 nick2 @nick1 ^M$ ---> 353, list all users in that channel, @is the channel creator
	// :London.UK.EU.StarLink.Org 366 nick1 #test :End of /NAMES list.^M$ ---> 366

Join::Join(Server &server, Client &client, std::string message) : Command(server, client , message){}

Join::~Join() {};

/* ---------------------- METHODS --------------------------------------------*/

int Join::executeCommand(){
    Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;
	if (Command::checkEmptyParamter())
		return 1;

	if (this->_paramVec[0].at(0) == 0) {
		//TODO kick client out of any channel
	}
	std::vector<std::string> channelsToJoin, keyForChannel;
	std::vector<Channel> joinChannel;
	
	channelsToJoin = parse_join_kick(this->_paramVec[0]);
	if (this->_paramVec.size() >= 2)
		keyForChannel = parse_join_kick(this->_paramVec[1]);

    std::vector<Channel> *channels = this->_server->getChannels();
	for (size_t j = 0; j < channelsToJoin.size(); j++) {
		int i = this->_server->channel_exists(channelsToJoin[j]);
		if (i == -1) {

			Channel channel(channelsToJoin[j]);
			channel.add_user(this->_client->getNickName(), "", true);
			this->_server->addChannel(channel);
			std::string msg = make_msg_ready(j, "");
			
			this->_server->send_message_to_channel(msg, channel);
            Command::returnMsgToServer(RPL_NAMREPLY(this->_server->getHostname(), this->_client->getNickName(), channel.get_name(), "", channel.get_creator()));
            Command::returnMsgToServer(RPL_ENDOFNAMES(this->_server->getHostname(), channel.get_creator(), channel.get_name()));
		}
		else {
			int code;
			if (j < keyForChannel.size()) 
				code = (*channels)[i].add_user(this->_client->getNickName(), keyForChannel[j], false);
			else
				code = (*channels)[i].add_user(this->_client->getNickName(), "", false);
				
			//switchcase
			if (code)
				std::cout << std::endl;//REPLY with adequate numReply
			else {
				std::string msg = make_msg_ready(j, "");
				this->_server->send_message_to_channel(msg, (*channels)[i]);
                Command::returnMsgToServer(RPL_NAMREPLY(this->_server->getHostname(), this->_client->getNickName(), (*channels)[i].get_name(), (*channels)[i].make_memberlist(), (*channels)[i].get_creator()));
                Command::returnMsgToServer(RPL_ENDOFNAMES(this->_server->getHostname(), (*channels)[i].get_creator(), (*channels)[i].get_name()));
			} 
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
    return 0;
}

std::vector<std::string> Join::parse_join_kick(std::string commaToken) {
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

std::string Join::make_msg_ready(size_t channelnumber, std::string topic_message){
    std::string msg;
	(void) topic_message;

    msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" " + this->_command + " " + this->_paramVec[channelnumber];
    return (msg);
}
