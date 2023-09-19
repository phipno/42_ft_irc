/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:27:08 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/19 15:32:59 by kczichow         ###   ########.fr       */
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
#include "Client.Class.hpp"
#include <unistd.h>

#define MAX_CONNECTIONS 100
#define MAX_EVENTS 100

class Server{

    private:
        int							_port;
        std::string					_password;

        int							_serverSocket;
		struct sockaddr_in			_serverAddr;
		struct pollfd				_serverPollfd;
		std::vector<Client *>			_clients;
		std::vector<pollfd>			_fds;
		nfds_t						_nfds;
		// pthread_mutex_t				_clientMutex;
		Server();

		int setupServer();
		void acceptNewClient();
		void handleClient(Client *client);
		void cleanupClients();

	public:
		~Server();
		Server(int port, std::string password);
		Server (Server const &src);
		Server &operator= (Server const &src);

		void runServer();
};