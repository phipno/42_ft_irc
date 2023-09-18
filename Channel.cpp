#include "Channel.hpp"

Channel::Channel() : _channelName("Unknown"), _topicMessage("None"), \
					 _topic_restricted(true), _invite_only(false), \
					_passPhrase(""), _userlimit(-1) {}

Channel::Channel(std::string name) : _channelName(name), _topicMessage("None"), \
					 _topic_restricted(false), _invite_only(false), \
					_passPhrase(""), _userlimit(-1) {}

Channel::Channel(std::string name, bool topic, bool invite, std::string pass, int userlimit) : \
					_channelName(name),  _topicMessage("None"), \
					_topic_restricted(topic), _invite_only(invite), \
					_passPhrase(pass), _userlimit(userlimit) {}


//priviliges are given to a user
int Channel::give_priveleges(std::string cli) {

	if (VERBOSE)
		std::cout << "give_privileges" << std::endl;

	std::map<std::string, bool>::iterator it = _users.find(cli);
	if (it != _users.end()) {
		it->second = true;
		return (1);
	}
	else {
		if (DEBUG)
			std::cout << "No User found, returned 0" << std::endl;
		return (0);
	}
}

// priviliges are removed
int Channel::rm_priveleges(std::string cli) {

	if (VERBOSE)
		std::cout << "remove_privileges" << std::endl;

	std::map<std::string, bool>::iterator it = _users.find(cli);
	if (it != _users.end()) {
		it->second = false;
		return (1);
	}
	else {
		if (DEBUG)
			std::cout << "No User found, returned 0" << std::endl;
		return (0);
	}
}

//A user joins WITHOUT privilieges into this channel
int Channel::join(std::string cli) {

	if (VERBOSE)
		std::cout << "join" << std::endl;

	std::map<std::string, bool>::iterator it = _users.find(cli);
	if (it == _users.end()) {
		_users[cli] = false;
		return (1);
	}
	else {
		if (DEBUG)
			std::cout << "User already exists, returned 0" << std::endl;
		return (0);
	}
}

//A User gets kicked from the channel.
int Channel::kick(std::string to_kick) {
	
	if (VERBOSE)
		std::cout << "kick" << std::endl;

	std::map<std::string, bool>::iterator it = _users.find(to_kick);
	if (it != _users.end()) {
		_users.erase(to_kick);
		return (1);
	}
	else {
		if (DEBUG)
			std::cout << "User not found, returned 0" << std::endl;
		return (0);
	}
}

//A User gets invited to the channel
// int Channel::invite(std::string to_invite) {
	
// 	if (VERBOSE)
// 		std::cout << "invite" << std::endl;

// 	std::map<std::string, bool>::iterator it = _users.find(to_invite);
// 	if (it != _users.end()) {
// 		_users.(to_kick);
// 		return (1);
// 	}
// 	else {
// 		if (DEBUG)
// 			std::cout << "User not found, returned 0" << std::endl;
// 		return (0);
// 	}
// }

int Channel::topic(std::string topic_message) {
	
	if (VERBOSE)
		std::cout << "Topic Message" << std::endl;

	set_topic(topic_message);
	return (1);
}

//==============================================================================
//Debugging
void Channel::list_clients(void) {

	if (VERBOSE)
		std::cout << "list_clients" << std::endl;

	std::map<std::string, bool>::iterator it = _users.begin();
	for ( ; it != _users.end(); it++) {
		std::cout << "Client: " << it->first << " Privilieges: " << it->second << std::endl;
	}
	std::cout << "--------------------" << std::endl;
}

void Channel::list_channel_attributes(void) {

	if (VERBOSE)
		std::cout << "list_attributes" << std::endl;

		std::cout << "ChannelName: " << _channelName << std::endl;
		std::cout << "TopicMessage: " << _topicMessage << std::endl;
		std::cout << "Topic restricted?: " << _topic_restricted << std::endl;
		std::cout << "Invite Only? " << _invite_only << std::endl;
		std::cout << "Password: " << _passPhrase << std::endl;
		std::cout << "Userlimit: " << _userlimit << std::endl;
	std::cout << "--------------------" << std::endl;

}


//==============================================================================
//setter
void Channel::set_users(std::map<std::string, bool> users) {_users = users;}
void Channel::set_topic(std::string message) {_topicMessage = message;}
void Channel::set_topic_restriction(bool restriction) {_topic_restricted = restriction;}
void Channel::set_invite_only(bool restriction) {_invite_only = restriction;}
void Channel::set_passphrase(std::string passphrase) {_passPhrase = passphrase;}
void Channel::set_userlimit(int limit) {_userlimit = limit;}

//getter
std::map<std::string, bool> Channel::get_users(void) {return (_users);}
std::string Channel::get_topic(void) {return (_topicMessage);}
bool Channel::get_topic_restriction(void) {return (_topic_restricted);}
bool Channel::get_invite_only(void) {return (_invite_only);}
std::string Channel::get_passphrase(void) {return (_passPhrase);}
int Channel::get_userlimit(void) {return (_userlimit);}