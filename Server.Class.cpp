/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:01 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/22 12:41:34 by aestraic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.Class.hpp"

/* ---------------- CANONICAL FORM ---------------------------*/
Server::Server() : _port(0), _password("no_pw"){
	this->_fds[MAX_EVENTS];
};

Server::Server(int port, std::string password) : _port(port), _password(password){};

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
void Server::recv_from_client_socket(Client &client){
	char buffer[512];

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
	
	}
}

//A message is written to a client's socket
void Server::send_msg_to_client_socket(Client &client) {
	
	char msg[8] = "Message";
	int bytesRead = send(client.getClientSocket(), msg, sizeof(msg), 0);
	(void) bytesRead;
	// if (bytesRead == -1)
		// perror("send message to client");
}

void Server::acceptNewClient() {

	if (VERBOSE)
		std::cout<< "acceptNewClient()" << std::endl;
		
	Client     newClient;
    socklen_t   clientAddrLen = sizeof(newClient.getClientAddr());
	
    newClient.setClientSocket(accept(this->_serverSocket, reinterpret_cast<struct sockaddr *>(&newClient.getClientAddr()), &clientAddrLen));
	
	if (newClient.getClientSocket() == -1)
    	perror("accept");

	if (newClient.getClientSocket() == -1){
        std::cerr << "Error accepting client connection\n";
    }

	//insert prompt and checks for nickname, username and password;
    std::cout << "Client connected\n";

	newClient.setClientPollfdFD(newClient.getClientSocket());
	newClient.setClientPollfdEvents(POLLIN);

    // set new client to non-blocking mode
    // fcntl(newClient.getClientSocket(), F_SETFL, O_NONBLOCK);

    // add new client socket to pollfds and client to clients vector
    this->_clients.push_back(newClient);
    this->_fds.push_back(newClient.getClientPollfd());

    std::cout << "new client added to list of known clients on socket " << newClient.getClientSocket() << " at address " << &_clients.back() << "\n";
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

void Server::runServer(){

	if (VERBOSE)
		std::cout<< "runServer()" << std::endl;
	setupServer();

	while (true){
		// set poll with unlimited time
		int PollResult = poll(_fds.data(), _fds.size(), -1);
		if (PollResult == -1){
			perror("poll");
			continue;
		}

		//used for connecting of a client ot a server
		if (this->_fds[0].revents & POLLIN){
			acceptNewClient();
		}

		for (unsigned long i = 0; i < _fds.size(); ++i) {
			
			//messages from the client are sent directly to the server's socket
			if(_fds[i + 1].revents & POLLIN){ 
				recv_from_client_socket(_clients[i]);
			}
			
			send_msg_to_client_socket(_clients[i]);
			if(_fds[0].revents & POLLOUT) {
				recv_from_client_socket(_clients[i]);
			}
        // command if pressed CTRL + C
            // receive / send
        // remove client (close)
		}
	}
	for (int i = 1; i < _clients.size(); ++i){
		close(_clients[i].getClientSocket());
	}
	close (_serverSocket);
};