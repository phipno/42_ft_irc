/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:40:27 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/18 13:43:34 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Client{

		protected:
			struct sockaddr_in	_clientAddr;
			int					_clientSocket;
			pollfd				_clientPollfd;

		public:
			Client();
			~Client();
			Client (Client const &src);
			Client &operator= (Client const &src);

			struct sockaddr_in &getClientAddr();
			int &getClientSocket();
			pollfd &getClientPollfd();
			
			void setClientAddr(struct sockaddr_in clientAddr);
			void setClientSocket(int clientSocket);
			void setClientPollfd (pollfd clientPollfd);
		
};