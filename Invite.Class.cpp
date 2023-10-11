#include "Invite.Class.hpp"

/*
   Parameters: <nickname> <channel>
   The INVITE command is used to invite a user to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  However, if the channel exists, only members of the channel
   are allowed to invite other users.  When the channel has invite-only
   flag set, only channel operators may issue INVITE command.

   Only the user inviting and the user being invited will receive
   notification of the invitation.  Other channel members are not
   notified.  (This is unlike the MODE changes, and is occasionally the
   source of trouble for users.)
*/

Invite::Invite(Server &server, Client &client, std::string message) : Command(server, client , message){}

Invite::~Invite() {};

/* ---------------------- METHODS --------------------------------------------*/


int Invite::executeCommand(){

    Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;
	if (checkEmptyParamter())
		return 1;

	std::string channelName = this->_paramVec[1];
	std::string inviteNick = this->_paramVec[0];

    std::vector<Client> clients = this->_server->getClients();
	std::vector<Client>::iterator clientit = clients.begin();
	for ( ; clientit < clients.end(); clientit++){
		if (clientit->getNickName() == inviteNick){
			break;
		}
	}
	if (clientit == clients.end())
        Command::numReply(ERR_NOSUCHNICK(this->_server->getHostname(), inviteNick));

	std::vector<Channel> *channels = this->_server->getChannels();
    std::vector<Channel>::iterator it = channels->begin();
	for (; it != channels->end(); it++){
		if (it->get_name() == channelName){
			Channel channel = *it;
			if (channel.is_in_channel(inviteNick)){
				Command::numReply(ERR_USERONCHANNEL(this->_server->getHostname(), inviteNick, channel.get_name()));
                return 1;
			}
			if (channel.get_invite_only()){
				if (channel.is_operator(this->_client->getNickName())){
					Command::numReply(RPL_INVITING(this->_server->getHostname(), this->_client->getNickName(), inviteNick, channel.get_name()));
                    std::string msg = make_msg_ready(0, "");
					it->set_invitee(inviteNick);
					return 0;
				}
				else{
					Command::numReply(ERR_CHANOPRIVSNEEDED(this->_server->getHostname(), this->_client->getNickName(), channel.get_name()));
                    return 1;
				}
			}
			else {
				if (channel.is_in_channel(this->_client->getNickName())){
					Command::numReply(RPL_INVITING(this->_server->getHostname(), this->_client->getNickName(), inviteNick, channel.get_name()));
                    std::string msg = make_msg_ready(0, "");
					this->_server->send_msg_to_client_socket(*clientit, msg);
					it->set_invitee(inviteNick);
					return 0;
				}
				else {
					Command::numReply(ERR_NOTONCHANNEL(this->_server->getHostname(), this->_client->getNickName(), channel.get_name()));
					return 1;
				}
			}
		}
	}
    Command::numReply(RPL_INVITING(this->_server->getHostname(), this->_client->getNickName(), inviteNick, channelName));
	std::string msg = make_msg_ready(0, "");
	this->_server->send_msg_to_client_socket(*clientit, msg);
	return 0;
}

int Invite::checkEmptyParamter(){
	std::cout << MAGENTA << "EMPTYPARAM\n" << RESET;
	if (this->_paramVec.empty() || this->_paramVec.size() == 1) {
		Command::numReply(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), this->_command));
		return (1);
	}
	return (0);
}

std::string Invite::make_msg_ready(size_t channelnumber, std::string topic_message){
	std::string msg;
	(void) channelnumber;
	(void) topic_message;
	msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" " + this->_command + " " + this->_paramVec[0] + " :" + this->_paramVec[1];
	return (msg);
}