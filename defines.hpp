/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:03:14 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/28 11:23:57 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

//protocol errors
#define MAX_CONNECTIONS 100
#define MAX_EVENTS 100

#define RPL_WELCOME(hostname, nickname, username) (std::string(": " + this->_hostname + " 001 " + client.getNickName() + " :Welcome to the Internet Relay Network, " + client.getNickName() + "!" +client.getUserName() + "@" + this->_hostname))
// #define RPL_YOURHOST 002
// #define RPL_CREATED 003
// #define RPL_INVITING 341
// #define ERR_NOSUCHNICK 401
// #define ERR_NOSUCHCHANNEL 403
// #define ERR_NONICKNAMEGIVEN 431
// #define ERR_ERRONEUSNICKNAME 432
// #define ERR_ALREADYREGISTRED 462
// #define ERR_NEEDMOREPARAMS 461
// #define ERR_NICKNAMEINUSE 433
// #define ERR_NORECIPIENT 411
// #define ERR_NOTEXTTOSEND 412
// #define ERR_USERONCHANNEL 443
// #define ERR_USERONCHANNEL(nickname, channel) (std::string("irc42 443 " + nick + " " + channel +  " :is already on channel"))
// std::string RPL_WELCOME(std::string prefix) { return ":Welcome to the Internet Relay Network " + prefix; }

// #define ERR_NOTONCHANNEL 442 //"<channel> :You're not on that channel"
// #define ERR_NOCHANMODES 477 //"<channel> :Channel doesn't support modes"

//TOPIC errors              
// #define RPL_NOTOPIC 331 //"<channel> :No topic is set"              
// #define RPL_TOPIC 332 //topic is set
// #define ERR_CHANOPRIVSNEEDED 482 //"<channel> :You're not channel operator"          

//custom errors
#define ERR_PING 005

//Ping/Pong Message
#define PONG 1234567890
#define ERR_NOORIGIN 409 //":No origin specified"

//debugflags
#define VERBOSE 0 //used for constructors and function calls
#define LOOP 0 //used for printing when inside a loop
#define DEBUG 1 // generic print flag