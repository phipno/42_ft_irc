#include "Nick.Class.hpp"

/* NICK

*/

Nick::Nick(Server &server, Client &client, std::string message) : Command(server, client , message){}

Nick::~Nick() {};

/* ---------------------- METHODS --------------------------------------------*/


int Nick::executeCommand(){

	Command::tokenizeMsg();
	if (Command::checkRegistrationStatus())
		return 1;
	if (checkEmptyParamter())
		return 1;

	std::string allowed_chars = "abcdefghijklmnopqrstuvwxyz0123456789{}[]\\|";
	for (size_t i = 0; i < this->_paramVec[0].length(); ++i){
        char ch = this->_paramVec[0][i];
		if (allowed_chars.find(ch) != std::string::npos){}
        else {
			Command::numReply(ERR_ERRONEUSNICKNAME(this->_server->getHostname(), this->_client->getNickName()));
			return 1;
        }
	}
	std::cout << MAGENTA << "CHECK 1\n" << RESET;
    // check if nickname already exists on same server
	if (Command::isValidNickname()){
		Command::numReply(ERR_NICKNAMEINUSE(this->_server->getHostname(), this->_client->getNickName()));
		return 1;
	}

	std::cout << MAGENTA << "CHECK 2\n" << RESET;
	//if superuser, he will be welcomed.
	if (this->_client->getNickName() == "superuser") {
		Command::numReply(RPL_WELCOME(this->_server->getHostname(), this->_client->getNickName(), this->_client->getUserName()));
		Command::numReply(RPL_YOURHOST(this->_server->getHostname(), this->_client->getNickName()));
		this->_client->getSu();
		this->_client->registerClient(SUPERUSER);
		return (0);
	}
	std::cout << MAGENTA << "CHECK 3\n" << RESET;
	// if client typed om username, he will be welcomed after nickname
	if (this->_client->getStatus() == USERNAME) {
		this->_client->setNickName(this->_paramVec[0]);
		Command::numReply(RPL_WELCOME(this->_server->getHostname(), this->_client->getNickName(), this->_client->getUserName()));
		Command::numReply(RPL_YOURHOST(this->_server->getHostname(), this->_client->getNickName()));
		this->_client->registerClient(WELCOMED);
		return (0);
	}
	else {
		this->_client->setNickName(this->_paramVec[0]);
		this->_client->registerClient(NICKNAME);
	}
	std::cout << MAGENTA << "CHECK 4\n" << RESET;
	return 0;
}

int Nick::checkEmptyParamter(){
	std::cout << MAGENTA << "EMPTYPARAM\n" << RESET;
	if (this->_paramVec.empty()){
		Command::numReply(ERR_NONICKNAMEGIVEN(this->_server->getHostname()));
		return (1);
	}
	return (0);
}