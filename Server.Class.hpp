/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/12 11:14:23 by kczichow         ###   ########.fr       */
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
#include <sstream>

#include "defines.hpp"
#include "Channel.Class.hpp"
#include "Client.Class.hpp"

typedef	struct s_msg{
	std::string					prefix;
	std::string					command;
	std::string					param;
	std::vector<std::string>	paramVec;
	
} t_msg;

class Server{

	private:
		int						_port;
		std::string				_password;
		std::string				_hostname;
		int						_serverSocket;
		struct sockaddr_in		_serverAddr;
		struct pollfd			_serverPollfd;
		std::vector<Channel>	_channels;
		std::vector<Client>		_clients;
		std::vector<pollfd>		_fds;
		t_msg					_parMsg;
		
		Server();
		Server (Server const &src);
		Server &operator= (Server const &src);
		
		void acceptNewClient();
		int setupServer();
	
	public:
		~Server();
		Server(int port, std::string password);

		void runServer();
		int pong(t_msg *message, class Client &client);
		int handshake(t_msg *message, class Client &client);

		//Messages
		std::string recv_from_client_socket(Client &client);
		void send_msg_to_client_socket(Client &client, std::string message);
		void send_message_to_channel(std::string message, class Channel &channel);
		void numReply(Client &client, std::string message);

		//Channels
		void join_channel(std::string channelName, class Client &client);
		int channel_exists(std::string channelName);
		
		//Parsing
		void parsing_msg(std::string &message, Client &client);
	 	void executeCommands(Client &client, std::string Message);

		//Debugging
		void list_channels(void);
		void list_channels_all(void);
		void list_clients(void);

		//Getter/Setter
		std::vector<Client> get_clients(void);
		std::vector<pollfd> get_fds(void);
		int get_serversocket(void);
		t_msg get_parsedMsg();
		std::string getPassword();
		std::string getHostname();
		std::vector<Client> getClients();
		std::vector<Channel> *getChannels();
		void addChannel(Channel channel);
};