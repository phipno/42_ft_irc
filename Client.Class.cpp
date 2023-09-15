/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:46:35 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/15 14:17:23 by kczichow         ###   ########.fr       */
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

void Client::setClientAddr(struct sockaddr_in clientAddr){
	this->_clientAddr = clientAddr;
}

void Client::setClientSocket(int clientSocket){
	this->_clientSocket = clientSocket;
};
