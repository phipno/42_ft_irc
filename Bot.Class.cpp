#include "Bot.Class.hpp"

Bot::Bot(Server &server, Client &client, std::string message) :Command(server, client, message)  {

	_db.push_back("https://www.youtube.com/watch?v=NOtLb34Osl0");
	_db.push_back("https://www.youtube.com/watch?v=jfKfPfyJRdk");
	_db.push_back("https://www.youtube.com/watch?v=jfKfPfyJRdk");
	_db.push_back("https://www.youtube.com/watch?v=xPU8OAjjS4k");
	_db.push_back("https://www.youtube.com/watch?v=c4dV9MBm3LQ");
	_db.push_back("https://www.youtube.com/watch?v=civuoU_NE38");
	_db.push_back("https://www.youtube.com/watch?v=dD40VXFkusw");
	_db.push_back("https://www.youtube.com/watch?v=CpDcn6ox4kE");
	_db.push_back("https://www.youtube.com/watch?v=6Wm4c4N1zm0");
	_db.push_back("https://www.youtube.com/watch?v=DJ_xfwgQXXY");
	_db.push_back("https://www.youtube.com/watch?v=hMAPyGoqQVw");
}

Bot::~Bot() {}

int Bot::executeCommand() {
	Command::tokenizeMsg();
	if (Command::checkRegistrationStatusWelcomed())
		return 1;
	// if (Command::checkEmptyParamter())
	// 	return 1;

	//unsigned long i = randomzahl, zwischen 1 udn 11
	size_t i = 5;
	std::string msg = make_msg_ready(_db[i]);
	Command::numReply(msg);
	return (0);
}


std::string Bot::make_msg_ready(std::string yt_link) {
	
    std::string msg;
	msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" " + this->_command + " : Here check this out " + yt_link;
	
	return (msg);
}
