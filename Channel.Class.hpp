#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "defines.hpp"
#include "Client.Class.hpp"

class Channel {
	
	protected:
		std::string _channelName;
		std::string _creator; //creator of the channel
		std::map<std::string, bool> _users; //has all users and their operator flag
		std::vector<std::string>	_invited;
		std::string _topicMessage; // standard topic message is: "None"
		bool _topic_restricted; //if true only operators can alter the topic_message
		bool _invite_only;
		bool _pw_restricted;
		std::string _passPhrase; // if empty, the channel is not pw_limited
		int _userlimit; //if <=0, the channel is unlimited

	public:

		//Canonical Form
		Channel();
		Channel(std::string name);
		Channel(std::string name, bool topic, bool invite, std::string pass, int userlimit);

		
		int give_priveleges(std::string cli);
		int rm_priveleges(std::string cli);
		int add_user(std::string client, std::string pass, bool operatorflag); 
		bool is_in_channel(std::string name); 
		bool is_operator(std::string name); 
		std::string make_memberlist(void);
		int kick_user(std::string to_kick); //returns 1 if successfull
		bool has_permission(std::string name);

		//Debugging
		void list_clients_in_channel(void); 
		void list_channel_attributes(void);
		
		//setters
		void set_users(std::map<std::string, bool> users);
		void set_name(std::string name);
		void set_topic(std::string message);
		void set_topic_restriction(bool restriction);
		void set_invite_only(bool restriction);
		void set_passphrase(std::string passphrase);
		void set_passrestriction(bool restriction);
		void set_userlimit(int limit);
		void set_invitee(std::string);

		//getters
		std::map<std::string, bool> get_users(void);
		std::string get_name(void);
		std::string get_topic(void);
		bool get_topic_restriction(void);
		bool get_invite_only(void);
		std::string get_passphrase(void);
		int get_userlimit(void);
		bool get_passrestriction(void);
		std::string get_creator(void);
};