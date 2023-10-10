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
	Command::returnMsgToServer(msg);
	return (0);
}


std::string Bot::make_msg_ready(std::string yt_link) {
	
    std::string msg;
	msg += ":" + this->_client->getNickName() + "!~" + this->_client->getUserName() + "@" + this->_server->getHostname() + \
	" " + this->_command + " : Here check this out " + yt_link;
	
	return (msg);
}

// # include "Server.hpp"
// ​
// bool Server::parseReqQueue(Client& client)
// {
// 	for (std::vector<std::string>::iterator it = client.getReqQueue().begin(); it != client.getReqQueue().end(); ++it)
// 	{
// 		if (!this->parseReq(client, *it))
// 			return false;
// 	}
// 	client.getReqQueue().clear();
// 	return true;
// }
// ​
// bool Server::parseReq(Client& client, std::string request)
// {
// 	std::vector<std::string> reqVec;
// 	std::string reqField;
// ​
// 	std::istringstream iss(request);
// 	while (iss >> reqField)
// 		reqVec.push_back(reqField);
// ​
// 	if (reqVec.size())
// 	{
// 		reqVec[0] = toUpperCase(reqVec[0]);
// 		std::map<std::string, void(Server::*)(std::vector<std::string> reqVec, Client& client)>::iterator it = this->_commands.find(reqVec[0]);
		
// 		if (it != this->_commands.end() && (client.getUserRegistered() || reqVec[0] == "PASS" || reqVec[0] == "NICK" || reqVec[0] == "USER" || reqVec[0] == "CAP" || reqVec[0] == "PING" || reqVec[0] == "DBG"))
// 			(this->*(it->second))(reqVec, client);
// 		else
// 			this->sendResponse(client, E_NOTREGISTERED(client));
// ​
// 		if (reqVec[0] == "QUIT")
// 			return false;
// ​
// 	}
// 	return true;
// }
// ​
// bool Server::buildReqQueue(Client& client, const std::string& buffer)
// {
// 	std::istringstream iss(buffer);
// 	std::string buffer_str;
// ​
// 	while (std::getline(iss, buffer_str, '\n'))
// 	{
// 		client.getReqQueueBuf() += buffer_str;
// 		if (!iss.eof())
// 		{
// 			client.getReqQueue().push_back(client.getReqQueueBuf());
// 			client.getReqQueueBuf().clear();
// 		}
// 	}
// 	if (!buffer.empty() && buffer[buffer.size() - 1] == '\n')
// 		return true;
// ​
// 	return false;
// }
// ​
// bool Server::handleClientReq(Client& client)
// {
// 	std::vector<char> buffer(RECV_BUF);
// 	std::fill(buffer.begin(), buffer.end(), '\0');
// ​
// 	int recv_len = recv(client.getPollFd().fd, buffer.data(), RECV_BUF, 0);
// ​
// 	if (recv_len <= 0)
// 		return false;
// 	else
// 	{		
// 		std::string buf(buffer.begin(), buffer.begin() + recv_len);
		
// 		std::cout << BLUE << buf << RESET << std::flush;
// 		if (!this->buildReqQueue(client, buf))
// 			return true;
// 		if (!this->parseReqQueue(client))
// 			return false;
// 	}
// 	return true;
// }