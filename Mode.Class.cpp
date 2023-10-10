#include "Mode.Class.hpp"

/*
 	The MODE command is provided so that channel operators may change the
	characteristics of `their' channel:
	Parameters: <channel> {[+|-]|o|i|t|k|l} [<limit>] [<user>]

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

Mode::Mode(Server &server, Client &client, std::string message) : Command(server, client , message){}

Mode::~Mode() {};

/* ---------------------- METHODS --------------------------------------------*/

int Mode::executeCommand(){
    Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;
	if (Command::checkEmptyParamter())
		return 1;

	std::vector<std::string>::iterator params = this->_paramVec.begin();
	
	int i = this->_server->channel_exists(*params);
	if (i == -1) {
		Command::returnMsgToServer(ERR_NOSUCHNICK(this->_server->getHostname(), this->_client->getNickName()));
		return(0);
	}
	
	std::vector<Channel> *channels = this->_server->getChannels();
	std::vector<Channel>::iterator it = channels->begin();
	for (; it != channels->end(); it++){
		if (!(*channels)[i].is_operator(this->_client->getNickName())) {
			Command::returnMsgToServer(ERR_CHANOPRIVSNEEDED(this->_server->getHostname(), this->_client->getNickName(), (*channels)[i].get_name()));
			return (0);
		}
		std::string channel = *params;
		if (++params == this->_paramVec.end()) {
			Command::returnMsgToServer(RPL_CHANNELMODEIS(this->_server->getHostname(), (*channels)[i].get_name()));
			return (0);
		}
	}
	
	std::cout << "PARAMS: " << *params << std::endl;
	for (int i = 1; params != this->_paramVec.end(); params++, i++) {
		
		if (*params == "+i" || *params == "-i" || *params == "+t" || *params == "-t" )
			topic_invite_restriction(this->_paramVec, i);
		else if (*params == "+k" || *params == "-k")
			key_mode(this->_paramVec, i);
		else if (*params == "+l" || *params == "-l")
			user_limit(this->_paramVec, i);
		else if (*params == "+o" || *params == "-o")
			operator_mode(this->_paramVec, i);
	}
	return (0);
}

int Mode::topic_invite_restriction(std::vector<std::string> params, int pos) {

	int i = this->_server->channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator next = params.begin() + pos + 1;
	std::string msg;
	
	std::cout << "mode: " << std::endl;
	std::vector<Channel> *channels = this->_server->getChannels();
	std::vector<Channel>::iterator it = channels->begin();
	for (; it != channels->end(); it++){

		if (*mode == "+i" && (next == params.end() || is_in_modes(*next)))
			(*channels)[i].set_invite_only(true);
		if (*mode == "-i" && (next == params.end() || is_in_modes(*next)))
			(*channels)[i].set_invite_only(false);
		if (*mode == "+t" && (next == params.end() || is_in_modes(*next)))
			(*channels)[i].set_topic_restriction(true);
		if (*mode == "-t" && (next == params.end() || is_in_modes(*next)))
			(*channels)[i].set_topic_restriction(false);
	
		msg = make_msg_ready(0, *mode);
		this->_server->send_message_to_channel(msg, (*channels)[i]);
		return 0;
	}
	Command::returnMsgToServer(ERR_UNKNOWNMODE(this->_server->getHostname(), this->_client->getNickName(), *next));
	return (472);
}

int Mode::operator_mode(std::vector<std::string> params, int pos) {

	int i = this->_server->channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator user = params.begin() + pos + 1;

	if (user == params.end() || is_in_modes(*user)){
		Command::returnMsgToServer(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), "MODE +o"));
		return(461);
	}

	std::vector<Channel> *channels = this->_server->getChannels();
	for (int j = 0; user != params.end() && !is_in_modes(*user) && j < 3; j++, user++) {

		if ((*channels)[i].is_in_channel(*user)) {
			if (*mode == "+o")
				(*channels)[i].give_priveleges(*user);
			else
				(*channels)[i].rm_priveleges(*user);
		}
		else {
			if (*mode == "+o")
				Command::returnMsgToServer(ERR_USERNOTINCHANNEL(this->_server->getHostname(), this->_client->getNickName(), *user, params[0]));
			else
				Command::returnMsgToServer(ERR_USERNOTINCHANNEL(this->_server->getHostname(), this->_client->getNickName(), *user, params[0]));
		}
	}
	return (0);
}

int Mode::user_limit(std::vector<std::string> params, int pos) {

	int i = this->_server->channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator limit = params.begin() + pos + 1;
	std::string msg;
	std::vector<Channel> *channels = this->_server->getChannels();

	if (*mode == "+l" && (limit == params.end() || is_in_modes(*limit) || !valid_number(*limit, i))) {

		Command::returnMsgToServer(ERR_UNKNOWNMODE(this->_server->getHostname(), this->_client->getNickName(), *mode));
		return(461);
	}
	
	if (*mode == "+l" && (limit != params.end() && valid_number(*limit, i))) {

		(*channels)[i].set_userlimit(valid_number(*limit, i));
		msg = make_msg_ready(0, *mode);
		this->_server->send_message_to_channel(msg, (*channels)[i]);
		return(0);
	}
	else
		Command::returnMsgToServer(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), "+l"));

	if (*mode == "-l" && (limit == params.end() || is_in_modes(*mode))) {

		(*channels)[i].set_userlimit(-1);
		msg = make_msg_ready(0, *mode);
		this->_server->send_message_to_channel(msg, (*channels)[i]);
		return(0);
	}
	else
		Command::returnMsgToServer(ERR_UNKNOWNMODE(this->_server->getHostname(), this->_client->getNickName(), *limit));
	return (0);
}

int Mode::valid_number(std::string param, int channelindex) {

	std::vector<Channel> *channels = this->_server->getChannels();
	int integer = 0;
	int origin_limit = (*channels)[channelindex].get_userlimit();
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

int Mode::key_mode(std::vector<std::string> params, int pos) {

	int i = this->_server->channel_exists(params[0]);
	std::vector<std::string>::iterator mode = params.begin() + pos;
	std::vector<std::string>::iterator key = params.begin() + pos + 1;
	std::string msg;

	std::vector<Channel> *channels = this->_server->getChannels();
	if ((*channels)[i].get_passrestriction() && *mode == "+k") {

		Command::returnMsgToServer(ERR_KEYSET(this->_server->getHostname(), this->_client->getNickName(), (*channels)[i].get_name()));
		return (467);
	}
	
	if (*mode == "+k" && (key == params.end() || is_in_modes(*key) || !valid_passphrase(*key))) {

		Command::returnMsgToServer(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), "+k"));
		return(461);
	}

	if (*mode == "+k" && valid_passphrase(*key)) {
		(*channels)[i].set_passrestriction(true);
		(*channels)[i].set_passphrase(*key);
		msg = make_msg_ready(0, *mode);
		this->_server->send_message_to_channel(msg, (*channels)[i]);
		return (0);
	}

	if (*mode == "-k" && (is_in_modes(*key) || key == params.end())) {
		(*channels)[i].set_passrestriction(false);
		(*channels)[i].set_passphrase("");
		msg = make_msg_ready(0, *mode);
		this->_server->send_message_to_channel(msg, (*channels)[i]);
		return (0);
	}
	else
		Command::returnMsgToServer(ERR_UNKNOWNMODE(this->_server->getHostname(), this->_client->getNickName(), *key));
	return (0);
}

bool Mode::valid_passphrase(std::string param) {

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

bool Mode::is_in_modes(std::string param) {
	
	std::string modes[10] = {"+i", "-i", "+k", "-k", "+l", "-l", "+o", "-o", "+t", "-t"};
	for (int i = 0; i < 10; i++) {
		if (modes[i] == param)
			return (true);
	}
	return (false);
}

std::string Mode::make_msg_ready(size_t channelnumber, std::string topic_message){
    std::string msg;
	(void) topic_message;

    msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" MODE " + this->_paramVec[0] + " " + topic_message;
    return (msg);
}
