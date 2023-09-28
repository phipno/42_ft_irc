/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.operator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 14:11:56 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/28 15:19:17 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"


/* File contains commands for operators only */

/*
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
int Server::invite(t_msg *message, Client &client) {
	if (VERBOSE)
		std::cout << "invite" << std::endl;
	
	std::string channelName = message->paramVec[1];
	std::string inviteNick = message->paramVec[0];
	
	std::vector<Channel>::iterator it = _channels.begin();
	for (; it != _channels.end(); it++){
		if (it->get_name() == channelName){
			Channel channel = *it;
			if (channel.is_in_channel(inviteNick)){
				numReply(client, ERR_USERONCHANNEL(this->_hostname, client.getNickName(), channel.get_name()));
				return 1;
			}
			if (channel.get_invite_only()){
				if (channel.is_operator(client.getNickName())){
					numReply(client, RPL_INVITING(this->_hostname, client.getNickName(), channel.get_name()));
					it->set_invitee(inviteNick);
					return 0;
				}
				else{
					numReply(client, ERR_CHANOPRIVSNEEDED(this->_hostname, client.getNickName(), channel.get_name()));
					return 1;
				}
			}
			else {
				if (channel.is_in_channel(client.getNickName())){
					numReply(client, RPL_INVITING(this->_hostname, client.getNickName(), channel.get_name()));
					it->set_invitee(inviteNick);
					return 0;
				}
				else {
					numReply(client, ERR_NOTONCHANNEL(this->_hostname, client.getNickName(), channel.get_name()));					// numReply(ERR_NOTONCHANNEL, message, client);
					return 1;
				}
			}
		}
		numReply(client, RPL_INVITING(this->_hostname, client.getNickName(), channelName));
	}
	return 0;
}