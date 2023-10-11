#include "Command.Class.hpp"
#include "defines.hpp"

Command::Command(){}

Command::Command(Server &server, Client &client, std::string message) :  _message(message), _client(&client), _server(&server) {
	if (VERBOSE)
		std::cout << GRASS_GREEN << "Hello from Command constructor\n" << RESET;
}

Command::~Command(){
	if (VERBOSE)
		std::cout << GRASS_GREEN << "Hello from Command destructor\n" << RESET;
}

Command::Command (Command const &src){
	*this = src;
}

Command &Command::operator=(Command const &src){
	this->_message = src._message;
	this->_prefix = src._prefix;
	this->_command = src._command;
	this->_param = src._param;
	this->_paramVec = src._paramVec;
	this->_client = src._client;
	this->_server = src._server;
	this->_channels = src._channels;
	return (*this);
}
	
void Command::tokenizeMsg(){

	std::cout << "message is: " << this->_message << std::endl;

	std::stringstream tempSS(this->_message);
	std::string       Token;
	std::string       String;
	bool              ignore_space = false;

	tempSS >> Token;
	this->_command = Token;
	while (tempSS >> Token) {
		if (Token[0] == ':')
			ignore_space = true;
		if (ignore_space == false)
			_paramVec.push_back(Token);
		else
			String += Token + " ";
	}
	//removes space from last token
	String = String.substr(0, String.size() - 1);
	if (ignore_space == true)
		_paramVec.push_back(String);

	if (DEBUG) {
      std::cout << " .Com: " << this->_command << std::endl;
      for (size_t i = 0; i < this->_paramVec.size(); ++i) {
        std::cout << i << ".Tok: " << this->_paramVec[i] << std::endl;
      }
	}
}

std::vector<std::string> Command::parse_join_kick(std::string commaToken) {
	std::vector<std::string>	splitToken;
	size_t	pos;

	while ((pos = commaToken.find(',')) != std::string::npos) {
		splitToken.push_back(commaToken.substr(0, pos));
		commaToken.erase(0, pos + 1);
	}
	if (pos == std::string::npos)
		splitToken.push_back(commaToken);

	return splitToken;
}

int Command::numReply(std::string message){
	this->_server->send_msg_to_client_socket(*this->_client, message);
	return (1);
}

int Command::checkRegistrationStatus(){
	std::cout << MAGENTA << "REGSTATUS\n" << RESET;
	// std::cout << MAGENTA << this->_client->getRegistrationStatus() << RESET;
	if (this->_client->getRegistrationStatus() < REGISTERED){
		numReply(ERR_ALREADYREGISTERED(this->_server->getHostname(), this->_client->getNickName()));
		return (1);
	}
	return (0);
}

int Command::checkRegistrationStatusWelcomed(){
	std::cout << MAGENTA << "REGSTATUS\n" << RESET;
	// std::cout << MAGENTA << this->_client->getRegistrationStatus() << RESET;
	if (this->_client->getRegistrationStatus() < WELCOMED){
		numReply(ERR_NOTREGISTERED(this->_server->getHostname(), this->_client->getNickName()));
		return (1);
	}
	return (0);
}

int Command::checkEmptyParamter(){
	if (this->_paramVec.empty()){
		numReply(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), this->_command));
		return (1);
	}
	return (0);
}

int Command::isValidNickname(){
	std::vector<Client> clients = this->_server->getClients();
	std::vector<Client>::iterator it = clients.begin();

	for (;it != clients.end(); it++){
		if (it->getNickName() == this->_paramVec[0]){
			return 1;
        }
	}
	return 0;
}