/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:01 by kczichow          #+#    #+#             */
/*   Updated: 2023/10/05 13:50:10 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.Class.hpp"
bool g_sigint = false;

/* ---------------- CANONICAL FORM ---------------------------*/
Server::Server(){
	if (VERBOSE)
		std::cout << MAGENTA << "Hello from Server constructor\n" << RESET;
	this->_fds[MAX_EVENTS];
};

Server::Server(int port, std::string password) : _port(port), _password(password), _hostname("Ingwer.Radish.Cherry"){
};

Server::~Server(){
	if (VERBOSE)
		std::cout << MAGENTA << "Hello from Server destructor\n" << RESET;
};

Server::Server (Server const &src){
	if (VERBOSE)
		std::cout << MAGENTA << "Hello from Server copy constructor\n" << RESET;
	*this = src;
};

Server &Server::operator= (Server const &src){
    this->_port = src._port;
    this->_password = src._password;
	this->_hostname = src._hostname;
    this->_serverSocket = src._serverSocket;
	this->_serverAddr = src._serverAddr;
	this->_serverPollfd = src._serverPollfd;
	this->_channels = src._channels;
	this->_clients = src._clients;
	this->_fds = src._fds;
	this->_parMsg = src._parMsg;
    return (*this);
};


/* ------------------------- PRIVATE METHODS ---------------------------------*/
//sets up a new connection between client and server socket
void Server::acceptNewClient(void) {

	if (VERBOSE)
		std::cout<< "acceptNewClient()" << std::endl;

	std::string nickName;	
	std::string userName;	
	Client     newClient;
	socklen_t  clientAddrLen = sizeof(newClient.getClientAddr());
	
    newClient.setClientSocket(accept(this->_serverSocket, reinterpret_cast<struct sockaddr *>(&newClient.getClientAddr()), &clientAddrLen));
	if (newClient.getClientSocket() == -1)
    	perror("accept client connection");
    std::cout << "Client socket connected" << std::endl;
	send_msg_to_client_socket(newClient, "Enter Password:");
	
	//insert prompt and checks for nickname, username and password;
	newClient.setClientPollfdFD(newClient.getClientSocket());
	newClient.setClientPollfdEvents(POLLIN | POLLOUT);

    // set new client to non-blocking mode
    // fcntl(newClient.getClientSocket(), F_SETFL, O_NONBLOCK);
		
	//init new client
	newClient.setSu(false);
	newClient.setStatus(0);
	
	this->_fds.push_back(newClient.getClientPollfd());
	this->_clients.push_back(newClient);
    std::cout << "new client socket added to list of known sockets" << newClient.getClientSocket() << " at address " << &_clients.back() << "\n";
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

/* ------------------------- PUBLIC METHODS ----------------------------------*/

void Server::runServer() {

	if (VERBOSE)
		std::cout<< "runServer()" << std::endl;

	unsigned long i; //clients[i]
	unsigned long j; //_fds[j]
	std::string cmd;

	setupServer();
	
	while (!g_sigint) {
		// std::cout << "run_Server()" << g_sigint << std::endl;
		// set poll with unlimited time
		int PollResult = poll(_fds.data(), _fds.size(), -1);
		if (PollResult == -1) {
			perror("poll");
			continue;
		}
		// std::cout << "poll_result" << PollResult << std::endl;
		
		
		//used for connecting a client to a server
		if (this->_fds[0].revents & POLLIN){
			acceptNewClient();
		}
		if (this->_fds[0].revents & POLLHUP){
			// for (unsigned int i = 0; i < get_clients().size(); i++) {
			// 	close(get_clients()[i].getClientSocket());
			// }
			close(get_serversocket());
		}
		for (i = 0, j = 1; j < _fds.size() && i < _clients.size(); i++, j++) {
					
			if(_fds[j].revents & POLLIN) {
				recv_from_client_socket(_clients[i]);
				list_clients();
				list_channels_all();
				std::cout << "========================" << std::endl;
			}
			if (_fds[j].revents & POLLHUP) {
				
				// remove_client(_clients[i], i);
				close(_clients[i].getClientSocket());
				_clients.erase(_clients.begin() + i);
				_fds.erase(_fds.begin() + j);
				std::cout << "Deleting client" << std::endl;
			}
		}
	}
};

// void Server::remove_client(Client &client, int client_id) {
	
// 	remove_client_from_channels(client);
// 	(void) client_id;
// 	// close(_clients[client_id].getClientSocket());
// 	// _clients.erase(_clients.begin() + client_id);
// 	// _fds.erase(_fds.begin() + client_id + 1);
// }

//TO-DO Removing works for _clients-vector, but cant remove clients from channels prperly
// void Server::remove_client_from_channels(class Client &client) {

// 	std::vector<Channel>::iterator it_chan = _channels.begin();
	
// 	for (int i = 0 ; it_chan != _channels.end(); it_chan++, i++) {
// 		if (it_chan->get_users().find(client.getNickName()) != it_chan->get_users().end())
// 			_channels[i].get_users().erase(_channels[i].get_users().find(client.getNickName()));
// 	}
// }

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
	else {
		std::string message(buffer);
		std::cout << "<<<<<<<" << message << std::endl;
		parsing_msg(message, client);
		return (message);
	}
	return (message);
}

//A message is written to a client's socket
void Server::send_msg_to_client_socket(Client &client, std::string message) {

	//make the message ready for kvirc
	
	int bytesRead = 0;
	
	bytesRead = send(client.getClientSocket(), (message + "\r\n").c_str() , message.length() + 2, 0);
	std::cout << ">>>>>>>" << message + "\r\n" << std::endl;
	
	if (bytesRead == -1)
		perror("send message to client");
}

//Debugging/ HELPER functions

//an empty string is, when there is only a : in the token
bool Server::is_empty_string(std::string token) {
	
	std::cout << "TOKEN: " << token << std::endl;
	// Last token has always a space at the end(should be fixed)
	if (token == ":") {
		std::cout << "empty string()" << std::endl;
		return (true);
	}
	return (false);
}

//sends a message to all memebers of the channel, except for oneself
void Server::send_message_to_channel(std::string message, class Channel &channel) {

	std::vector<Client>::iterator it = _clients.begin();

	for (int i = 0 ; it != _clients.end() ; it++, i++) {

		if (channel.is_in_channel(it->getNickName())) {
			send_msg_to_client_socket(_clients[i], message);
		}
	}
}

void Server::numReply(Client &client, std::string message){
	send_msg_to_client_socket(client, message);
}

void Server::list_channels(void) {

	std::vector<Channel>::iterator it = _channels.begin();
	std::cout << "------- list_channels() -------" << std::endl;
	for ( ; it != _channels.end(); it++) {	
		std::cout << it->get_name() << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
}

void Server::list_channels_all(void) {
	if (VERBOSE)
		std::cout << "------- list_channels_all() -------" << std::endl;
	std::vector<Channel>::iterator it = _channels.begin();
	for ( ; it != _channels.end(); it++) {	
		std::cout << "------- channels_name() -------" << std::endl;
		std::cout << it->get_name() << std::endl;
		it->list_channel_attributes();
		std::cout << "------- clients -------" << std::endl;
		it->list_clients_in_channel();
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

//getter/setter
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
std::string Server::getPassword(){
	return this->_password;
}

std::string Server::getHostname(){
	return this->_hostname;
}

std::vector<Client> Server::getClients(){
	return this->_clients;
}

std::vector<Channel> *Server::getChannels(){
	return &this->_channels;
}