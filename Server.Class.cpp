/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:01 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/26 12:37:36 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.Class.hpp"
extern bool g_sigint;

/* ---------------- CANONICAL FORM ---------------------------*/
Server::Server() : _port(0), _password("no_pw"), _hostname("Ingwer.Radish.Cherry"){
	this->_fds[MAX_EVENTS];
};

Server::Server(int port, std::string password) : _port(port), _password(password), _hostname("Ingwer.Radish.Cherry"){
};

Server::~Server(){};

Server::Server (Server const &src){ (void) src;};

Server &Server::operator= (Server const &src){
    this->_serverSocket = src._serverSocket;
    this->_port = src._port;
    this->_password = src._password;
    return (*this);
};


/* ------------------------- PRIVATE METHODS ---------------------------------*/

//receives a message from a client's socket
std::string Server::recv_from_client_socket(Client &client) {
	char buffer[512];
	std::string message;
	
	memset(buffer, 0, sizeof(buffer));
	int bytesRead = recv(client.getClientSocket(), buffer, sizeof(buffer), 0);
	
	if (bytesRead == -1) {
			// Handle other errors.
			perror("recv");
	} 
	else if (bytesRead == 0) {
		// The client has sent an empty string.
	} 
	else {
		// Process the received data.
		std::string message(buffer); 
		std::cout << "Received: " << message << std::endl;
		parsing_msg(message, client);
		return (message);
	}
	std::cout << "recv_from_client_socket():" << bytesRead << std::endl;
	return (message);
}

//A message is written to a client's socket
void Server::send_msg_to_client_socket(Client &client, std::string message) {

	int bytesRead = send(client.getClientSocket(), (message + "\r\n").c_str() , message.length() + 2, 0);
	if (bytesRead == -1)
		perror("send message to client");
}

//sets up a new connection between client and server socket
void Server::acceptNewClient() {

	if (VERBOSE)
		std::cout<< "acceptNewClient()" << std::endl;

	std::string nickName;	
	std::string userName;	
	Client     newClient;
	socklen_t  clientAddrLen = sizeof(newClient.getClientAddr());
	
    newClient.setClientSocket(accept(this->_serverSocket, reinterpret_cast<struct sockaddr *>(&newClient.getClientAddr()), &clientAddrLen));
	if (newClient.getClientSocket() == -1)
    	perror("accept client connection");
	
	// as long correct passphrase is not entered, asks for password
	while (newClient.getStatus() != 1) {
		
		send_msg_to_client_socket(newClient, "Enter Password:");
		recv_from_client_socket(newClient);
	}
	//insert prompt and checks for nickname, username and password;
    std::cout << "Client socket connected" << std::endl;
	
	newClient.setClientPollfdFD(newClient.getClientSocket());
	newClient.setClientPollfdEvents(POLLIN | POLLOUT);

    // set new client to non-blocking mode
    // fcntl(newClient.getClientSocket(), F_SETFL, O_NONBLOCK);
    // add new client socket to pollfds
    this->_fds.push_back(newClient.getClientPollfd());
	this->_clients.push_back(newClient);
	
	
    std::cout << "new client socket added to list of known sockets" << newClient.getClientSocket() << " at address " << &_clients.back() << "\n";
}

// //used for creating or joining a channel, depending if it is already existent
// void Server::join_channel(std::string channelName, class Client &client) {
	
// 	int i = channel_exists(channelName);
// 	std::cout << "Channel already exists? " << i << std::endl;
// 	if (i == -1) {
// 		Channel channel(channelName);
// 		channel.add_user(client.getNickName(), true);
// 		_channels.push_back(channel);
// 	}
// 	else{
// 		_channels[i].add_user(client.getNickName(), false);
// 	}
// }

//if a channel exists, it returns the index of the channel. Otherwise it returns a -1.
// int Server::channel_exists(std::string channelName) {

// 	std::vector<Channel>::iterator it = _channels.begin();
// 	for ( int i = 0; it != _channels.end(); it++, i++) {
// 		if (channelName == it->get_name())
// 			return (i);
// 	}
// 	return (-1);
// }

//sends a message to all memebers of the channel
void Server::send_message_to_channel(std::string message, class Channel &channel) {

	std::vector<Client>::iterator it = _clients.begin();

	for (int i = 0 ; it != _clients.end() ; it++, i++) {

		if (channel.is_in_channel(it->getNickName())) {
			send_msg_to_client_socket(_clients[i], message);
		}
	}
}



int Server::setupServer(){

	if (VERBOSE)
		std::cout<< "setupServer()" << std::endl;
	
	// create serversocket
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1){
		std::cerr << "Error creating socket\n" ;
		return 1;
	}

	// bind serversocket, using internet style (struct sockaddr_in)
	this->_serverAddr.sin_family = AF_INET;
	this->_serverAddr.sin_port = htons(this->_port);
	// this->_serverAddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(_serverSocket, reinterpret_cast<struct sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
		std::cerr << "Error binding socket\n";
		return 1;
	}

	// set server to listen
	if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1){
		std::cerr << "Error listening on socket\n";
		return 1;
	}

	// set serversocket to non-blocking mode
	// fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

	_serverPollfd.fd = this->_serverSocket;
	_serverPollfd.events = POLLIN | POLLOUT;
	this->_fds.push_back(this->_serverPollfd);
	// this->_nfds = 1;

	std::cout << "Server listening on port " << this->_port << std::endl;
	return 0;
}

void Server::handle_requests(t_msg request) {

	(void) request;
	// if (request.cmd == "JOIN")
		//join	
	//etc...
}

/* ------------------------- PUBLIC METHODS ----------------------------------*/

void Server::runServer() {

	if (VERBOSE)
		std::cout<< "runServer()" << std::endl;

	std::string cmd;

	setupServer();
	// Channel channel("Channel1");
	// _channels.push_back(channel);
	list_channels();
	
	while (!g_sigint) {
		// set poll with unlimited time
		
		int PollResult = poll(_fds.data(), _fds.size(), -1);
		if (PollResult == -1) {
			perror("poll");
			continue;
		}
		
		//used for connecting a client to a server
		if (this->_fds[0].revents & POLLIN){
			acceptNewClient();
		}
		unsigned long i; //clients[i]
		unsigned long j; //_fds[j]
		for (i = 0, j = 1; j < _fds.size() && i < _clients.size(); i++, j++) {
			//1. messages from the client are sent directly to the server's socket
			//2. ther server is processing the command
			//3. server sends a response
			// std::cout << "LOOP" << _fds.size() << std::endl;
			if(_fds[j].revents & POLLIN) {
				cmd = recv_from_client_socket(_clients[i]);
				// if (cmd == "JOIN\n") {
				// 	join_channel("Channel1", _clients[i]);
				// 	send_msg_to_client_socket(_clients[i], "U joined a channel");
				// }
				// send_message_to_channel("message to channel", _channels[0]);
				send_msg_to_client_socket(_clients[i], "hello from server");
				list_clients();
				list_channels_all();
				std::cout << "========================" << std::endl;
			}
		}
	}
};


//Debugging
void Server::list_channels(void) {

	std::vector<Channel>::iterator it = _channels.begin();
	std::cout << "------- list_channels() -------" << std::endl;
	for ( ; it != _channels.end(); it++) {	
		std::cout << it->get_name() << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
}
void Server::list_channels_all(void) {
	//TO-DO: Segfaults entfernen
	if (VERBOSE)
		std::cout << "------- list_channels_all() -------" << std::endl;
	std::vector<Channel>::iterator it = _channels.begin();
	for ( ; it != _channels.end(); it++) {	
		std::cout << "------- channels_name() -------" << std::endl;
		std::cout << it->get_name() << std::endl;
		std::cout << "------- clients -------" << std::endl;
		it->list_clients_in_channel(); // segfaults
	}
	std::cout << "--------------------------------" << std::endl;
}

void Server::list_clients(void) {

	std::vector<Client>::iterator it = _clients.begin();
	std::cout << "------- list_clients() -------" << std::endl;
	for ( ; it != _clients.end(); it++) {
		std::cout << "Nickname: " << it->getNickName() << std::endl;
		std::cout << "Username: " << it->getUserName() << std::endl;
		std::cout << "Status: " << it->getStatus() << std::endl;
		std::cout << "Superuser? " << it->getSu() << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
}

//getter/settter
std::vector<Client> Server::get_clients(void) {
	return (_clients);
}

std::vector<pollfd> Server::get_fds(void) {
	return (_fds);
}

int Server::get_serversocket(void) {
	return (_serverSocket);
}

t_msg Server::get_parsedMsg() {
	return _parMsg;
}
