/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnolte <pnolte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:08 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/25 10:59:14 by pnolte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <unistd.h>
#include <signal.h>

#include "defines.hpp"
#include "Channel.Class.hpp"
#include "Client.Class.hpp"
#include <unistd.h>

#define MAX_CONNECTIONS 100
#define MAX_EVENTS 100

#define RPL_WELCOME 001
#define RPL_YOURHOST 002
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_ALREADYREGISTRED 462
#define ERR_NEEDMOREPARAMS 461

typedef	struct s_msg{
	std::string					prefix; //dont know if i need that
	std::string					command;
	std::string					param; //this could be better formulated
	std::vector<std::string>	paramVec;
} t_msg;

class Server{

	private:
     int					_port;
     std::string			_password;
	 std::string			_hostname;
     int					_serverSocket;
	 struct sockaddr_in		_serverAddr;
	 struct pollfd			_serverPollfd;
	 std::vector<Channel>	_channels;
	 std::vector<Client>	_clients;
	 std::vector<pollfd>	_fds;
	 t_msg								_parMsg; //added this to private, just because :)
	
	 Server();
	 int setupServer();
	 void acceptNewClient();
	 void recv_from_client_socket(Client &client);
	 void send_msg_to_client_socket(Client &client, std::string message);
		
	public:
		~Server();
		Server(int port, std::string password);
		Server (Server const &src);
		Server &operator= (Server const &src);

		void runServer();
		std::string numReply(int errorCode, t_msg *message, Client client);

		std::vector<Client> get_clients();
		int get_serversocket();
		t_msg get_parsedMsg();
		void signal_handler(int binary);
	
		int nick(t_msg *message, Client &client);
		int pass(t_msg *message, Client &client);
	 
	 void parsing_msg(std::string &message, Client &client);
	 void executeCommands(Client &client);
};