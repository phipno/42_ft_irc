/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.Class.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kczichow <kczichow@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/29 13:39:43 by kczichow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>

#include "defines.hpp"
#define REGISTERED 1 //correct password entered
#define NICKNAME 2	 //nick name entered
#define USERNAME 3   //user name entered
#define WELCOMED 4   //once the user typed in his credentials and answered with PONG, he can use all commands
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
			bool				_superUser; //TO-DO(albert):nick must be superuser to be the serveradmin. For testing purposes only.
			
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
