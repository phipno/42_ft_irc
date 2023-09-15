/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 10:40:27 by kczichow          #+#    #+#             */
/*   Updated: 2023/09/15 14:19:50 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Client{

		private:
			struct sockaddr_in	_clientAddr;
			int					_clientSocket;

		public:
			Client();
			~Client();
			Client (Client const &src);
			Client &operator= (Client const &src);

			struct sockaddr_in &getClientAddr();
			int &getClientSocket();
			
			void setClientAddr(struct sockaddr_in clientAddr);
			void setClientSocket(int clientSocket);
		
};