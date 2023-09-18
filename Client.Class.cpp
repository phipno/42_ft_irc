/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:46:35 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/18 13:43:04 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.Class.hpp"

/* ------------------ CANONICAL FORM -----------------------------------------*/

Client::Client(){
	memset(&_clientAddr, 0, sizeof(_clientAddr));
};
Client::~Client(){};
Client::Client (Client const &src){
	*this = src;
};
Client &Client::operator= (Client const &src){
	(void) src;
	return (*this);
};

/* -------------------- METHODS ----------------------------------------------*/

struct sockaddr_in &Client::getClientAddr(){
	return (this->_clientAddr);
}

int &Client::getClientSocket(){
	return (this->_clientSocket);
};

pollfd &Client::getClientPollfd(){
	return (this->_clientPollfd);
};

void Client::setClientAddr(struct sockaddr_in clientAddr){
	this->_clientAddr = clientAddr;
}

void Client::setClientSocket(int clientSocket){
	this->_clientSocket = clientSocket;
};

void Client::setClientPollfd(pollfd clientPollfd){
	this->_clientPollfd = clientPollfd;
}
