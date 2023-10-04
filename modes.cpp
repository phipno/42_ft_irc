#include "defines.hpp"
#include "Server.Class.hpp"
#include "Client.Class.hpp"
#include "Channel.Class.hpp"

/*
	i: invite only
	t: topic restriction
	k: channel key
	o: operator privilege
	l: user limit

	ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
	ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
	ERR_NOTONCHANNEL                ERR_KEYSET
	RPL_BANLIST                     RPL_ENDOFBANLIST
	ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL
	ERR_USERSDONTMATCH              RPL_UMODEIS
	ERR_UMODEUNKNOWNFLAG
*/

int Server::mode(t_msg *message, Client &client){

	std::vector<std::string>::iterator params = message->paramVec.begin();


	//numReply: do nothing maybe
	if (params == message->paramVec.end())
		return (0);
	
	int i = channel_exists(*params);
	//numReply: no channel
	if (i == -1)
		return(0);
	
	//numReply(client, ERR_CHANOPRIVSNEEDED())
	if (!_channels[i].is_operator(client.getNickName()))
		return (0);

	//numReply: LIST the modes
	std::string channel = *params;
	if (++params == message->paramVec.end())
		return (0);

	for (int i = 0; params != message->paramVec.end(); params++) {
		
		if (*params == "+i" || *params == "-i" || *params == "+t" || *params == "-t" )
			topic_invite_restriction(*params, *(params + 1), channel);
		else if (*params == "+k" || *params == "-k")
			key_mode(*params, *(params + 1), channel);
		else if (*params == "+l" || *params == "-l")
			user_limit(*params, *(params + 1), channel);
		else if (*params == "+o" || *params == "-o")
			operator_mode(*params, *(params + 1), channel);
	}
}

int Server::operator_mode(std::string operatormode, std::string param, std::string channel) {

	int i = channel_exists(channel);

	
}

int Server::user_limit(std::string usermode, std::string param, std::string channel) {

	int i = channel_exists(channel);

	if (usermode == "+l" && (param.empty() || valid_number(param) == -1)) {

		//numReply(cliet, ERR_NEEDMOREPARAMS())
		return(461);
	}
	
	if (usermode == "-l" && (!is_in_modes(param) || !param.empty())) {

		//numReply(cliet, ERR_UNKNOWNMODE())
		return(472);
	}
	
	if (usermode == "+l" && valid_number(param)) {

		_channels[i].set_userlimit(valid_number(param));
		return(0);
	}
	if (usermode == "-l" && (is_in_modes(param) || param.empty())) {

		_channels[i].set_userlimit(-1);
		return(0);
	}
	return (0);
}

int Server::valid_number(std::string param) {

	int integer = -1;
	for(int i = 0 ; i < param.size() ; i++)
		if (param[i] < '0' || param[i] > '9')
			return (-1);

	if (!param.empty())
		integer = atoi(param.c_str());

	return (integer);
}

int Server::topic_invite_restriction(std::string topicmode, std::string param, std::string channel) {

	int i = channel_exists(channel);

	if (topicmode == "+i" && (is_in_modes(param) || param.empty())) {
		_channels[i].set_invite_only(true);
		return (0);
	}
	if (topicmode == "-i" && (is_in_modes(param) || param.empty())) {
		_channels[i].set_invite_only(false);
		return (0);
	}
	if (topicmode == "+t" && (is_in_modes(param) || param.empty())) {
		_channels[i].set_topic_restriction(true);
		return (0);
	}
	if (topicmode == "-t" && (is_in_modes(param) || param.empty())) {
		_channels[i].set_topic_restriction(false);
		return (0);
	}
	//numReply(cliet, ERR_UNKNOWNMODE())
	return (472);
}


int Server::key_mode(std::string keymode, std::string param, std::string channel) {

	int i = channel_exists(channel);

	if (_channels[i].get_passrestriction() && keymode == "+k") {

		// numReply(client, ERR_KEYSET())
		return (467);
	}
	
	if (keymode == "+k" && (param.empty() || is_in_modes(param) || !valid_passphrase(param))) {

		//numReply(cliet, ERR_NEEDMOREPARAMS())
		return(461);
	}

	if (keymode == "+k" && valid_passphrase(param)) {
		_channels[i].set_passrestriction(true);
		_channels[i].set_passphrase(param);
		return (0);
	}

	if (keymode == "-k" && (is_in_modes(param) || param.empty())) {
		_channels[i].set_passrestriction(false);
		_channels[i].set_passphrase("");
		return (0);
	}
	
	return (0);
}

bool Server::valid_passphrase(std::string param) {

    if (param.size() < 15) {

		for (char c : param) {
			if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
				c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '^' ||
				c == '&' || c == '*' || c == '(' || c == ')' || c == '-' || c == '_' ||
				c == '+' || c == '=' || c == '<' || c == '>' || c == '?'))
					return false;
		}
    	return true;
	}
	return (false);
}

bool Server::is_in_modes(std::string param) {
	
	std::string modes[10] = {"+i", "-i", "+k", "-k", "+l", "-l", "+o", "-o", "+t", "-t"};
	for (int i = 0; i < 10; i++) {
		if (modes[i] == param)
			return (true);
	}
	return (false);
}