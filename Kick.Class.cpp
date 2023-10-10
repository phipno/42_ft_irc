#include "Kick.Class.hpp"

//TO-DO: Send channelmessge:
	//:nick1!~user1@188.244.102.158 JOIN :#test^M$ --->nick1 joined the channel (channelmessage)
//additionally send a memberlist to that client only
	// :London.UK.EU.StarLink.Org 353 nick3 * #channel2 :nick3 nick2 @nick1 ^M$ ---> 353, list all users in that channel, @is the channel creator
	// :London.UK.EU.StarLink.Org 366 nick1 #test :End of /NAMES list.^M$ ---> 366

Kick::Kick(Server &server, Client &client, std::string message) : Command(server, client , message){}

Kick::~Kick() {};

/* ---------------------- METHODS --------------------------------------------*/

int Kick::executeCommand(){
    Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;
	if (Command::checkEmptyParamter())
		return 1;

    std::vector<std::string> channelsToKick, userToKick;
    std::vector<Channel> joinChannel;

    channelsToKick = Command::parse_join_kick(this->_paramVec[0]);
    if (this->_paramVec.size() >= 2)
        userToKick = Command::parse_join_kick(this->_paramVec[1]);

	std::vector<Channel> *channels = this->_server->getChannels();
    for (size_t j = 0; j < channelsToKick.size(); j++) {
        int i = this->_server->channel_exists(channelsToKick[j]);
        if (i == -1)
                ; //TODO numReply channel doesnt exist ERR_NOSUCHCHANNEL
        // ? is this client.getNickName good, or what NAME is saved inside the channels 
        else if ((*channels)[i].is_in_channel(this->_client->getNickName()) == false)
            ;//TODO error kicker is not in channel
        else if ((*channels)[i].has_permission(this->_client->getNickName()) == false)
            ;//TODO error has no permission
        else if (channelsToKick.size() == 1) {
            std::vector<std::string>::iterator it = userToKick.begin();
            for (; it != userToKick.end(); ++it) {
                if ((*channels)[i].is_in_channel(*it) == false)
                    ;//TODO error user to kick is not in channel
                else {
                    if (this->_paramVec.size() >= 3) {
                        (*channels)[i].kick_user(userToKick[j]);
                        Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nReason: " + this->_paramVec[2]);
                    } else {
                        (*channels)[i].kick_user(userToKick[j]);
                        Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nNo Reason given");
            }
                }
            }
        } else if (j > userToKick.size()){
            if (this->_paramVec.size() >= 3) {
                (*channels)[i].kick_user(userToKick[j]);
                Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nReason: " + this->_paramVec[2]);
            }
            else {
                (*channels)[i].kick_user(userToKick[j]);
                Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nNo Reason given");
            }
        } else
            ; //TODO error missing params
    }
	return 0;
}
