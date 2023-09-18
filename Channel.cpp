#include "Channel.hpp"

Channel::Channel() : _channelname("Unknown"), _topic_message("None"), \
					 _topic_restricted(true), _invite_only(false), \
					_passphrase(""), _userlimit(-1) {}

Channel::Channel(std::string name) : _channelname(name), _topic_message("None"), \
					 _topic_restricted(false), _invite_only(false), \
					_passphrase(""), _userlimit(-1) {}

Channel::Channel(std::string name, bool topic, bool invite, std::string pass, int userlimit) : \
					_channelname(name), _topic_restricted(topic),\
					_invite_only(invite),_passphrase(pass), \
					_userlimit(userlimit) {}


//when using find(), the ==operator must be overloaded to wirk with client classes aswell
int Channel::give_priveleges(class Client &cli) {

	// std::vector<Client>::iterator it = std::find(_users.begin(), _users.end(), cli);
	// if (it != _users.end()) {
		// it->_channels[_channelname]._operator = true;
		// return (1);
	// }
	// else
		return (0);
}

//when using find(), the ==operator must be overloaded to wirk with client classes aswell
int Channel::rm_priveleges(class Client &cli) {

	// std::vector<Client>::iterator it = std::find(_users.begin(), _users.end(), cli);
	// if (it != _users.end()) {
		// it->_channels[_channelname]._operator = false;
		// return (1);
	// }
	// else
		return (0);
}

// //when using find(), the ==operator must be overloaded to wirk with client classes aswell
int Channel::join(class Client &cli) {

	// std::vector<Client>::iterator it = std::find(_users.begin(), _users.end(), cli);
	_users.push_back(cli);
	return (1);
}

// //when using find(), the ==operator must be overloaded to wirk with client classes aswell
// int Channel::rm_user(class Client &cli) {

// 	std::vector<Client>::iterator it = std::find(_users.begin(), _users.end(), cli);
// 	_users.erase(cli);
// 	return (1);
// }

//setter
void Channel::set_users(std::vector<class Client> users) {_users = users;}
void Channel::set_topic(std::string message) {_topic_message = message;}
void Channel::set_topic_restriction(bool restriction) {_topic_restricted = restriction;}
void Channel::set_invite_only(bool restriction) {_invite_only = restriction;}
void Channel::set_passphrase(std::string passphrase) {_passphrase = passphrase;}
void Channel::set_userlimit(int limit) {_userlimit = limit;}

//getter
std::vector<class Client> Channel::get_users(void) {return (_users);}
std::string Channel::get_topic(void) {return (_topic_message);}
bool Channel::get_topic_restriction(void) {return (_topic_restricted);}
bool Channel::get_invite_only(void) {return (_invite_only);}
std::string Channel::get_passphrase(void) {return (_passphrase);}
int Channel::get_userlimit(void) {return (_userlimit);}