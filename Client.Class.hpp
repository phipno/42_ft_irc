/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aestraic <aestraic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/10/03 14:30:31 by aestraic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>

#include "defines.hpp"
#define REGISTERED 1 //correct password
#define NICKNAME 2
#define USERNAME 3  
#define WELCOMED 4   
#define SUPERUSER 5
#define USERLEN 9

class Client{

		protected:
			struct sockaddr_in	_clientAddr;
			struct pollfd		_clientPollfd;
			int					_clientSocket;
			std::string			_userName;
			std::string			_nickName;
			int					_status;
			bool				_superUser;
			
		public:
			Client();
			Client(std::string username, std::string nickname);
			Client (Client const &src);
			Client &operator= (Client const &src);
			~Client();

			void list_attributes();

			struct sockaddr_in &getClientAddr();
			int &getClientSocket();
			int &getClientPollfdFD();
			pollfd &getClientPollfd();
			int &getRegistrationStatus();
			std::string &getNickName();
			std::string &getUserName();
			int getStatus();
			bool getSu();
			
			void setClientAddr(struct sockaddr_in &clientAddr);
			void setClientSocket(int clientSocket);
			void setClientPollfdFD (int const &fd);
			void setClientPollfdEvents (int const &event);
			void registerClient(int status);
			void setNickName(std::string nickName);
			void setFullName(std::string fullName);
			void setStatus(int status);
			void setSu(bool su_flag);
};
