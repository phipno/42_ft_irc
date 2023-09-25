/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/25 14:25:26 by aestraic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Client.Class.hpp"

/* ------------------ CANONICAL FORM -----------------------------------------*/

															 
Client::Client():_userName("Unknown"), _nickName("Unknown") {
	memset(&_clientAddr, 0, sizeof(_clientAddr));
	
	if (VERBOSE)
		std::cout << "default constructor called\n"; 
};

Client::Client(std::string username, std::string nickname) : _userName(username),
															 _nickName(nickname) {}
															 
Client::~Client(){
	
	if (VERBOSE)
		std::cout << "destructor called\n";
};

Client::Client (Client const &src){
	
	if (VERBOSE)
		std::cout << "copy constructor called\n";
	if (this != &src)
		*this = src;
};

Client &Client::operator= (Client const &src){
	
	if (this != &src) {
		this->_clientAddr = src._clientAddr;
		this->_clientPollfd = src._clientPollfd;
		this->_clientSocket = src._clientSocket;
		this->_userName = src._userName;
		this->_nickName = src._nickName;
		this->_registered = src._registered;
	}
	return (*this);
};

/* -------------------- METHODS ----------------------------------------------*/

struct sockaddr_in &Client::getClientAddr(){
	return (this->_clientAddr);
}

int &Client::getClientSocket(){
	return (this->_clientSocket);
};

int &Client::getClientPollfdFD(){
	return (this->_clientPollfd.fd);
};

pollfd &Client::getClientPollfd(){
	return (this->_clientPollfd);
};

int &Client::getRegistrationStatus(){
	return (this->_status);
}

std::string &Client::getNickName(){
	return (this->_nickName);
};
std::string &Client::getUserName(){
	return (this->_userName);
};

void Client::setNickName(std::string nickName){
	_nickName = nickName;
}

void Client::setClientAddr(struct sockaddr_in &clientAddr){
	this->_clientAddr = clientAddr;
}

void Client::setClientSocket(int clientSocket){
	this->_clientSocket = clientSocket;
};

void Client::setClientPollfdFD(int const &fd){
	this->_clientPollfd.fd = fd;
}

void Client::setClientPollfdEvents(int const &events){
	this->_clientPollfd.events = events;
}

void Client::registerClient(int status){
	this->_status = status;
}

void Client::setNickName(std::string nickName){
	this->_nickName = nickName;
}
void Client::setFullName(std::string fullName){
	this->_userName = fullName;
}

															 
/*
3.3.1 Private messages

      Command: PRIVMSG
   Parameters: <msgtarget> <text to be sent>

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
*/
// std::string prvMessage(std::string message, class Client &cli) {
// 	void(message);
// 	void(cli);
// 	return (0);
// }
