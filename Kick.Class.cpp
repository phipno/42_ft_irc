#include "Kick.Class.hpp"
/*
   Command: KICK
   Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]

   The KICK command can be used to request the forced removal of a user
   from a channel.  It causes the <user> to PART from the <channel> by
   force.  For the message to be syntactically correct, there MUST be
   either one channel parameter and multiple user parameter, or as many
   channel parameters as there are user parameters.  If a "comment" is
   given, this will be sent instead of the default message, the nickname
   of the user issuing the KICK.

   The server MUST NOT send KICK messages with multiple channels or
   users to clients.  This is necessarily to maintain backward
   compatibility with old client software.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

//TO-DO: Send channelmessge:
	//:nick1!~user1@188.244.102.158 JOIN :#test^M$ --->nick1 joined the channel (channelmessage)
//additionally send a memberlist to that client only
	// :London.UK.EU.StarLink.Org 353 nick3 * #channel2 :nick3 nick2 @nick1 ^M$ ---> 353, list all users in that channel, @is the channel creator
	// :London.UK.EU.StarLink.Org 366 nick1 #test :End of /NAMES list.^M$ ---> 366

*/

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
    // std::vector<Channel> joinChannel;

    channelsToKick = Command::parse_join_kick(this->_paramVec[0]);
    if (this->_paramVec.size() >= 2) // ? 
        userToKick = Command::parse_join_kick(this->_paramVec[1]);

	std::vector<Channel> *channels = this->_server->getChannels();
    for (size_t j = 0; j < channelsToKick.size(); j++) {
        int i = this->_server->channel_exists(channelsToKick[j]);
        if (i == -1)
             Command::numReply(ERR_NOSUCHCHANNEL(this->_server->getHostname(), (*channels)[i].get_name()));
        else if((*channels)[i].is_operator(this->_client->getNickName())){
            std::vector<std::string>::iterator it = userToKick.begin();
            for (; it != userToKick.end(); ++it) {
                if ((*channels)[i].is_in_channel(*it) == false)
                    Command::numReply(ERR_USERNOTINCHANNEL(this->_server->getHostname(), it->data(), " ", (*channels)[i].get_name()));
                else {
                    if (this->_paramVec.size() >= 3) {
                        (*channels)[i].kick_user(userToKick[j]);
                        Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nReason: " + this->_paramVec[2]);
                    } else {
                        (*channels)[i].kick_user(userToKick[j]);
                        Command::numReply("You were kicked out of #" + (*channels)[i].get_name() + "\nNo Reason given");
            }
        }

        else
            Command::numReply(ERR_CHANOPRIVSNEEDED(this->_server->getHostname(), this->_client->getNickName(), (*channels)[i].get_name()));

        else if ((*channels)[i].is_in_channel(this->_client->getNickName()) == false)
            ;//TODO error kicker is not in channel
        else if ((*channels)[i].has_permission(this->_client->getNickName()) == false)
            ;//TODO error has no permission
        else if (channelsToKick.size() == 1) {
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
