/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:03:14 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/30 19:19:34 by aestraic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

//protocol errors
#define MAX_CONNECTIONS 100
#define MAX_EVENTS 100

#define RPL_WELCOME(hostname, nickname, username) (std::string(":" + hostname + " 001 " + nickname + " :Welcome to the Internet Relay Network, " + nickname + "!" +username + "@" +hostname))
#define RPL_YOURHOST(hostname, nickname) (std::string(":" + hostname + " 002 " + nickname + " :Your host is " + hostname + ", running on version 1.0 !"))
// what is content of RPL_INVITING?
#define RPL_INVITING(hostname, nickname, channel) (std::string(":" + hostname + " 341 " + nickname + " " + channel))

#define ERR_NOSUCHNICK(hostname, nickname) (std::string(":" + hostname + " 401 " + nickname + " :No such nick/channel"))
#define ERR_CANNOTSENDTOCHAN(hostname, nickname, channel) (std::string(":" + hostname + " 404 " + nickname + " " + channel + " :Cannot send to channel"))
#define ERR_NORECIPIENT(hostname, nickname, command) (std::string(":" + hostname + " 411 " + nickname + " " + command + " :No recipient given"))
// #define ERR_NOTEXTTOSEND(hostname, nickname) (std::string(":irc42 412 " + nick + " :No text to send"))
#define ERR_NOTEXTTOSEND(hostname, nickname) (std::string(":" + hostname + " 412 " + nickname + " :No text to send"))
#define ERR_NONICKNAMEGIVEN(hostname) (std::string(":" + hostname + " 431 :No Nickname given"))
#define ERR_ERRONEUSNICKNAME(hostname, nickname) (std::string(":" + hostname + " 432 " + nickname + " :Erroneus nickname"))
#define ERR_NICKNAMEINUSE(hostname, nickname) (std::string(":" + hostname + " 433 " + nickname + " :Nickname is already in use"))
#define ERR_NOTONCHANNEL(hostname, nickname, channel) (std::string(":" + hostname + " 442 " + nickname + " " + channel + " :You're not on that channel"))
#define ERR_USERONCHANNEL(hostname, nickname, channel) (std::string(":" + hostname + " 443 " + nickname + " " + channel + " :is already on channel"))
#define ERR_NOTREGISTERED(hostname, nickname) (std::string(":" + hostname + " 451 " + nickname + " :You have not registered"))
#define ERR_NEEDMOREPARAMS(hostname, nickname, command) (std::string(":" + hostname + " 461 " + nickname + " :" +command + " :Not enough parameters"))
#define ERR_ALREADYREGISTERED(hostname, nickname) (std::string(":" + hostname + " 462 " + nickname + " :You may not reregister"))
#define ERR_CHANOPRIVSNEEDED(hostname, nickname, channel) (std::string(":" + hostname + " 482 " + nickname + " " + channel + " :You're not channel operator"))

//TOPIC
#define RPL_NOTOPIC(hostname, nickname, channel) (std::string(":" + hostname + " 331 " + nickname + " " + channel + " :No topic is set"))         
#define RPL_TOPIC(hostname, nickname, channel, topic) (std::string(":" + hostname + " 332 " + nickname + " " + channel + " :" + topic))

//CHANNELS
#define RPL_NAMREPLY(hostname, nickname, channel, memberlist, creator) (std::string(":" + hostname + " 353 " + nickname + " = " + channel + " :" + memberlist + "@" + creator + " "))
#define RPL_ENDOFNAMES(hostname, creator, channel) (std::string(":" + hostname + " 366 " + creator + " " + channel + " :END of /NAMES list."))

//MODES
// #define ERR_NOCHANMODES 477 //"<channel> :Channel doesn't support modes"       

//custom errors
#define ERR_PING 005

//Ping/Pong Message
#define ERR_NOORIGIN 409 //":No origin specified"

//debugflags
#define VERBOSE 0 //used for constructors and function calls
#define LOOP 0 //used for printing when inside a loop
#define DEBUG 1 // generic print flag

// 	case ERR_PING:
	// 		reply = ": To connect, type PONG 1234567890";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case ERR_NOORIGIN:
	// 		reply = ": To connect, type PONG 1234567890";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;

	// 	case ERR_CHANOPRIVSNEEDED:
	// 		reply = ": You're not channel operator";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case RPL_TOPIC:
	// 		reply = ": <channel> :Topic is <TOPIC message>";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;
	// 	case RPL_NOTOPIC:
	// 		reply = ": <channel> :No topic is set";
	// 		send_msg_to_client_socket(client, reply);
	// 		break;