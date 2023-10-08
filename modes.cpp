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

int Server::mode(t_msg *message, Client &client) {

	std::vector<std::string>::iterator params = message->paramVec.begin();

	if (params == message->paramVec.end()) {
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), message->command));
		return (0);
	}
	
	int i = channel_exists(*params);
	if (i == -1) {
		numReply(client, ERR_NOSUCHNICK(this->_hostname, client.getNickName()));
		return(0);
	}
		
	if (!_channels[i].is_operator(client.getNickName())) {
		numReply(client, ERR_CHANOPRIVSNEEDED(this->_hostname, client.getNickName(), _channels[i].get_name()));
		return (0);
	}
	
	std::string channel = *params;
	if (++params == message->paramVec.end()) {
		numReply(client, RPL_CHANNELMODEIS(this->_hostname, _channels[i].get_name()));
		return (0);
	}
	
	std::cout << "PARAMS: " << *params << std::endl;
	for (int i = 1; params != message->paramVec.end(); params++, i++) {
		
		if (*params == "+i" || *params == "-i" || *params == "+t" || *params == "-t" )
			topic_invite_restriction(message->paramVec, i, client);
		else if (*params == "+k" || *params == "-k")
			key_mode(message->paramVec, i, client);
		else if (*params == "+l" || *params == "-l")
			user_limit(message->paramVec, i, client);
		else if (*params == "+o" || *params == "-o")
			operator_mode(message->paramVec, i, client);
	}
	return (0);
}

int Server::topic_invite_restriction(std::vector<std::string> params, int pos, class Client &client) {

	int i = channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator next = params.begin() + pos + 1;
	std::string msg;

	std::cout << "mode: " << std::endl;
	
	if (*mode == "+i" && (next == params.end() || is_in_modes(*next))) {
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		_channels[i].set_invite_only(true);
		return (0);
	}
	if (*mode == "-i" && (next == params.end() || is_in_modes(*next))) {
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		_channels[i].set_invite_only(false);
		return (0);
	}
	if (*mode == "+t" && (next == params.end() || is_in_modes(*next))) {
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		_channels[i].set_topic_restriction(true);
		return (0);
	}
	if (*mode == "-t" && (next == params.end() || is_in_modes(*next))) {
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		_channels[i].set_topic_restriction(false);
		return (0);
	}
	numReply(client, ERR_UNKNOWNMODE(this->_hostname, client.getNickName(), *next));
	return (472);
}

int Server::operator_mode(std::vector<std::string> params, int pos, class Client &client) {

	int i = channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator user = params.begin() + pos + 1;

	if (user == params.end() || is_in_modes(*user)){
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), "MODE +o"));
		return(461);
	}
	for (int j = 0; user != params.end() && !is_in_modes(*user) && j < 3; j++, user++) {

		if (_channels[i].is_in_channel(*user)) {
			if (*mode == "+o")
				_channels[i].give_priveleges(*user);
			else
				_channels[i].rm_priveleges(*user);
		}
		else {
			if (*mode == "+o")
				numReply(client, ERR_USERNOTINCHANNEL(this->_hostname, client.getNickName(), *user, params[0]));
			else
				numReply(client, ERR_USERNOTINCHANNEL(this->_hostname, client.getNickName(), *user, params[0]));
		}
	}
	return (0);
}

int Server::user_limit(std::vector<std::string> params, int pos, class Client &client) {

	int i = channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator limit = params.begin() + pos + 1;
	std::string msg;

	if (*mode == "+l" && (limit == params.end() || is_in_modes(*limit) || !valid_number(*limit, i))) {

		numReply(client, ERR_UNKNOWNMODE(this->_hostname, client.getNickName(), *mode));
		return(461);
	}
	
	if (*mode == "+l" && (limit != params.end() && valid_number(*limit, i))) {

		_channels[i].set_userlimit(valid_number(*limit, i));
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		return(0);
	}
	else
		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), "+l"));

	if (*mode == "-l" && (limit == params.end() || is_in_modes(*mode))) {

		_channels[i].set_userlimit(-1);
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		return(0);
	}
	else
		numReply(client, ERR_UNKNOWNMODE(this->_hostname, client.getNickName(), *limit));
	return (0);
}

int Server::valid_number(std::string param, int channelindex) {

	int integer = 0;
	int origin_limit = _channels[channelindex].get_userlimit();
	(void) origin_limit;
	unsigned long i = 0;

	for(i = 0 ; i < param.size() ; i++)
		if (param[i] < '0' || param[i] > '9')
			break ;

	if (i != param.size())
		return (0);

	integer = atoi(param.c_str());
	return (integer);
}

int Server::key_mode(std::vector<std::string> params, int pos, class Client &client) {

	int i = channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator key = params.begin() + pos + 1;
	std::string msg;

	if (_channels[i].get_passrestriction() && *mode == "+k") {

		numReply(client, ERR_KEYSET(this->_hostname, client.getNickName(), _channels[i].get_name()));
		return (467);
	}
	
	if (*mode == "+k" && (key == params.end() || is_in_modes(*key) || !valid_passphrase(*key))) {

		numReply(client, ERR_NEEDMOREPARAMS(this->_hostname, client.getNickName(), "+k"));
		return(461);
	}

	if (*mode == "+k" && valid_passphrase(*key)) {
		_channels[i].set_passrestriction(true);
		_channels[i].set_passphrase(*key);
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		return (0);
	}

	if (*mode == "-k" && (is_in_modes(*key) || key == params.end())) {
		_channels[i].set_passrestriction(false);
		_channels[i].set_passphrase("");
		msg = make_msg_ready(&this->_parMsg, client, 0, *mode);
		send_message_to_channel(msg, _channels[i]);
		return (0);
	}
	else
		numReply(client, ERR_UNKNOWNMODE(this->_hostname, client.getNickName(), *key));
	return (0);
}

bool Server::valid_passphrase(std::string param) {

    if (param.size() < 15) {

		for (unsigned long i = 0, c = param[i]; i < param.size(); i++) {
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
