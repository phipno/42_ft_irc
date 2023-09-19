#pragma once

#include <iostream>
#include <string>
#include <map>

class Channel;

// typedef struct s_info {
	
// 	bool _operator;
// 	bool _kicked;

// } t_info;

class Client {
	
	protected:
		std::string _userName;
		std::string _nickName;
	
	public:
		Client();
		Client(std::string username, std::string fullname);
		
		std::string prvMessage(std::string message, class Client &cli); //sends a private message to an user
		std::string messAge(std::string message, class Channel &chnl); //sends a message to every user inside the channel


};