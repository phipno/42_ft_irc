/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.operator.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 14:11:56 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/28 09:52:45 by kczichow         ###   ########.fr       */
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
	(void) client;
	std::vector<Channel>::iterator it = _channels.begin();
	for (; it != _channels.end(); it++){
		if (it->get_name() == message->paramVec[1]){
			Channel channel = *it;
			if (it->is_in_channel(message->paramVec[0])){
				numReply(ERR_USERONCHANNEL, message, client);
				return 1;
			}
			if (channel.get_invite_only()){
				// if client is channel operator -> function to be buil in channel class
				// RPL_INVITING
				// client add to invite vector
			// else
				// ERR_CHANOPRIVSNEEDED
			}
			else{
				if (channel.is_in_channel(client.getNickName())){
					// RPL_INVITING
				}
				else {
					// ERR_NOTONCHANNEL
				}
				
			}
			break;
		}
	}
	

	
	
	
	// std::map<std::string, bool>::iterator it = _users.find(to_invite);
	// if (it != _users.end()) {
	// 	_users.(to_kick);
	// 	return (1);
	// }
	// else {
	// 	if (DEBUG)
	// 		std::cout << "User not found, returned 0" << std::endl;
	return (0);
}