/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:08 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/25 13:43:25 by aestraic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
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
		std::string					prefix;
		std::string					command;
		std::string					param;
		std::vector<std::string>	paramVec;
} t_msg;

class Server{

    private:
        int							_port;
        std::string					_password;
		std::string					_hostname;

        int							_serverSocket;
		struct sockaddr_in			_serverAddr;
		struct pollfd				_serverPollfd;
		std::vector<Channel>		_channels;
		std::vector<Client>			_clients;
		std::vector<pollfd>			_fds;
		
		Server();
		int setupServer();
		void acceptNewClient();
		
	public:
		~Server();
		Server(int port, std::string password);
		Server (Server const &src);
		Server &operator= (Server const &src);

		//Requests and messages
		void handle_requests(t_msg request);
		std::string recv_from_client_socket(Client &client);
		void send_msg_to_client_socket(Client &client, std::string message);

		//Channels
		void send_message_to_channel(std::string message, class Channel &channel);		
		void join_channel(std::string channelName, class Client &client);
		int channel_exists(std::string channelName);
		int pass(t_msg *message, Client client);
			
		void runServer();
		std::string numReply(int errorCode, t_msg *message, Client client);

		//misc
		void signal_handler(int binary);

		//Debugging
		void list_channels(void);
		void list_clients(void);

		//Getters
		std::vector<Client> get_clients(void);
		std::vector<pollfd> get_fds(void);
		int get_serversocket(void);
};