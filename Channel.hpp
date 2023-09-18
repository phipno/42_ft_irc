#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Client.hpp"

class Channel {
	
	protected:
		std::string _channelname;
		std::vector<class Client*> _users; //has all users and their additional information, unique usernames are requiered
		std::string _topic_message; // standard topic message is: "None"
		bool _topic_restricted; //if true only operators can alter the topic_message
		bool _invite_only;
		std::string _passphrase; // if empty, the channel is not pw_limited
		int _userlimit; //if <=0, the channel is unlimited

	public:
		int give_priveleges(class Client &cli); //changes operator/kick-flag or adds a user if not in the container, returns 1 if succesfull
		int rm_priveleges(class Client &cli); //changes operator/kick-flag or adds a user if not in the container, returns 1 if succesfull
		
		Channel();
		Channel(std::string name);
		Channel(std::string name, bool topic, bool invite, std::string pass, int userlimit);

		
		int join(class Client &cli);
		
		//operator functions
		std::string kick(class Client &to_kick, class Channel &chnl); //returns the message, e.g. USER has been kicked from channel
		std::string invite(class Client &to_invite, class Channel &chnl); //returns the invite message
		std::string topic(std::string topic_message, class Channel &chnl); // if no message, then it returns the current emssage, otherwise it sets the topic_message
		//alters the mode of a channel, e.g. #teest +l 10 (limit of 10 users on channel #teest)
		class Channel& modes(char alter, std::string mode, class Channel &chnl);
		

		//setters
		void set_users(std::vector<class Client> users);
		void set_topic(std::string message);
		void set_topic_restriction(bool restriction);
		void set_invite_only(bool restriction);
		void set_passphrase(std::string passphrase);
		void set_userlimit(int limit);
		//getters
		std::vector<class Client> get_users(void);
		std::string get_topic(void);
		bool get_topic_restriction(void);
		bool get_invite_only(void);
		std::string get_passphrase(void);
		int get_userlimit(void);

};