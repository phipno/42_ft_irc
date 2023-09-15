/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:01 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/15 15:12:44 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.Class.hpp"

/* ---------------- CANONICAL FORM ---------------------------*/
Server::Server() : _port(0), _password("no_pw"){};
Server::Server(int port, std::string password) : _port(port), _password(password){
    if (pthread_mutex_init(&this->_clientMutex, NULL) != 0) {
            std::cerr << "Mutex initialization failed" << std::endl;
        }
};
Server::~Server(){
    pthread_mutex_destroy(&this->_clientMutex);
};
Server::Server (Server const &src){};
Server &Server::operator= (Server const &src){
    this->_serverSocket = src._serverSocket;
    this->_port = src._port;
    this->_password = src._password;
    return (*this);
};


/* ---------------- PRIVATE METHODS ---------------------------*/

int Server::setup(){
    close (_serverSocket);
     // create serversocket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1){
        std::cerr << "Error creating socket\n" ;
        return 1;
    }

    // bind serversocket, using internet style (struct sockaddr_in), Apple specific?
    this->_serverAddr.sin_family = AF_INET;
    this->_serverAddr.sin_port = htons(this->_port);
    this->_serverAddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
    // serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_serverSocket, reinterpret_cast<struct sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        return 1;
    }

    if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1){
        std::cerr << "Error listening on socket\n";
        return 1;
    }

    // set serversocket to non-blocking mode
    fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

    this->_run = true;
    std::cout << "Server listening on port " << this->_port << std::endl;

    // int poll (struct pollfd *fds, nfds_t nfds, int timeout);
    struct pollfd pollfds[MAX_EVENTS];

    std::cout << &poll << "\n";
    pollfds[0].fd = this->_serverSocket;
    pollfds[0].events = POLLIN;

    nfds_t nfds = 1;
    std::cout << pollfds[0].fd << "\n";
    std::cout << pollfds[0].events << "\n";
    std::cout << pollfds[0].revents << "\n";

    while (true){
        // poll with inlimited time
        int PollResult = poll(pollfds, nfds, -1);
        if (PollResult == -1){
            perror("poll");
            continue;
        }
        // new client (accept)
        if (pollfds[0].revents & POLLIN){
        /* code */
            Client      newClient;
            socklen_t   clientAddrLen = sizeof(newClient.getClientAddr());
            newClient.setClientSocket(accept(this->_serverSocket,reinterpret_cast<struct sockaddr *>(&newClient.getClientAddr()), &clientAddrLen));
            if (newClient.getClientSocket() == -1){
                std::cerr << "Error accepting client connection\n";
            }
            this->_clients.push_back(newClient);
            std::cout << "Client connected\n";

            // set new client to non-blocking mode
            fcntl(newClient.getClientSocket(), F_SETFL, O_NONBLOCK);

            // add new client sockets to pollfds
            
            pollfds[nfds].fd = newClient.getClientSocket();
            pollfds[nfds].events = POLLIN;
            nfds++;

        }
        for (int i = 1; i < nfds; ++i){
            if(pollfds[i].revents & POLLIN){
                _clients[i].setClientSocket(pollfds[i].fd);
                
            char buffer[1024];
            while (true){
                memset(buffer, 0, sizeof(buffer));
                int bytesRead = recv(_clients[i].getClientSocket(), buffer, sizeof(buffer), 0);
                // std::cout << bytesRead << std::endl;
                if (bytesRead == 0){
                    // remove client
                    close(_clients[i].getClientSocket());
                    std::cout << "Client disconnected" << std::endl;
                    // close (_serverSocket);
                    // break ;
                }
                if (bytesRead >0){
                std::string message(buffer);
                std::cout << "received: " << message << std::endl;
                }
            }
            
            }
        }

        // command 
            // receive / send
        // remove client (close)
    }
    close (_serverSocket);
    return 0;
};