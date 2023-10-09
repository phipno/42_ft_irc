#include "Pass.Class.hpp"

/* PASS
first step, even before connecting with a nickname
entry from client: PASS <YourServerPassword>
variables:
	command = PASS
	paramVec[0] = YourServerPassword

	462	ERR_ALREADYREGISTRED
	461	ERR_NEEDMOREPARAMS
*/

Pass::Pass(Server &server, Client &client, std::string message) : Command(server, client, message){}

Pass::~Pass() {};

/* ---------------------- METHODS --------------------------------------------*/


int Pass::executeCommand(){

	Command::tokenizeMsg();
	if (checkRegistrationStatus())
		return 1;
	if (checkEmptyParamter())
		return 1;
	std::cout << _server->getPassword() << std::endl;
	if (this->_paramVec[0].compare(_server->getPassword()) == 0){
		this->_client->registerClient(REGISTERED);
		std::cout << "Registering succesfull: Client status is " << this->_client->getStatus() << std::endl;
	}
	return (0);
}

int Pass::checkEmptyParamter(){
	std::cout << MAGENTA << "EMPTYPARAM\n" << RESET;
	if (this->_paramVec.empty()){
		Command::returnMsgToServer(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), this->_command));
		return (1);
	}
	return (0);
}

int Pass::checkRegistrationStatus(){
	std::cout << MAGENTA << "REGSTATUS\n" << RESET;
	if (this->_client->getRegistrationStatus() >= REGISTERED){
		returnMsgToServer(ERR_ALREADYREGISTERED(this->_server->getHostname(), this->_client->getNickName()));
		return (1);
	}
	return (0);
}
