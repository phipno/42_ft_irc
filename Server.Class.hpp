/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/30 19:48:05 by aestraic         ###   ########.fr       */
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
	 t_msg					_parMsg; //added this to private, just because :)
	
	 Server();
	 int setupServer();
	 void acceptNewClient();
		
	public:
		~Server();
		Server(int port, std::string password);
		Server (Server const &src);
		Server &operator= (Server const &src);

		void runServer();
		void ping(class Client &client);
		int pong(t_msg *message, class Client &client);
		int handshake(t_msg *message, class Client &client);
		std::string make_msg_ready(t_msg *message, Client &client, size_t channelnumber, std::string topic_message);

		//Messages
		std::string recv_from_client_socket(Client &client);
		void send_msg_to_client_socket(Client &client, std::string message);
		void send_message_to_channel(std::string message, class Channel &channel);		
		void list(t_msg &message, Client &client);

		//Commands
		void join_channel(std::string channelName, class Client &client);
		int pass(t_msg *message, Client &client);
		int nick(t_msg *message, Client &client);
		int user(t_msg *message, Client &client);
		int privmsg(t_msg *message, Client &client);
		void join(t_msg &parsedMsg, Client &client);
		int topic(t_msg *parsedMsg, Client &client);
		int mode(t_msg *parsedMsg, Client &client);
		int invite(t_msg *message, Client &client);

		//misc
		void signal_handler(int binary);
		void numReply(Client &client, std::string message);
		
		//parsing
		void parsing_msg(std::string &message, Client &client);
	 	void executeCommands(Client &client);
		int channel_exists(std::string channelName);
		bool is_empty_string(std::string token);

		//Debugging
		void list_channels(void);
		void list_channels_all(void);
		void list_clients(void);

		//Getters
		std::vector<Client> get_clients(void);
		std::vector<pollfd> get_fds(void);
		int get_serversocket(void);
		t_msg get_parsedMsg();
};