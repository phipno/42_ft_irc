#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
#include <sstream>

#include "defines.hpp"
#include "Command.Class.hpp"
#include "Server.Class.hpp"
#include "Pass.Class.hpp"
#include "Nick.Class.hpp"
#include "User.Class.hpp"
#include "PrivMsg.Class.hpp"
#include "Join.Class.hpp"
#include "Invite.Class.hpp"
#include "Topic.Class.hpp"
#include "Kick.Class.hpp"
#include "Mode.Class.hpp"

void  Server::executeCommands(Client &client, std::string Message) {

	Command *command = NULL;

	if (this->_parMsg.command == "PASS")
		command = new Pass(*this, client, Message);
	else if (this->_parMsg.command == "NICK")
		command = new Nick(*this, client, Message);
	else if (this->_parMsg.command == "USER")
		command = new User(*this, client, Message);
	else if (this->_parMsg.command == "JOIN")
		command = new Join(*this, client, Message);
	else if (this->_parMsg.command == "PRIVMSG")
		command = new PrivMsg(*this, client, Message);
	else if (this->_parMsg.command == "KICK")
		command = new Kick(*this, client, Message);
	else if (this->_parMsg.command == "TOPIC")
		command = new Topic(*this, client, Message);
	else if (this->_parMsg.command == "INVITE")
		command = new Invite(*this, client, Message);
  else if (this->_parMsg.command == "MODE")
		command = new Mode(*this, client, Message);
  else if (this->_parMsg.command == "PING")
		  pong(&this->_parMsg, client);
	if (command){
		command->executeCommand();
		delete command;
	} else if (this->_parMsg.command != "NOTICE" && this->_parMsg.command != "PING" \
          && this->_parMsg.command != "WHO" && this->_parMsg.command != "PART")
    numReply(client, ERR_UNKNOWNCOMMAND(this->_hostname, this->_parMsg.command));
}

t_msg tokenize_msg(std::string Message) {
  std::stringstream tempSS(Message);
  std::string       Token;
  std::string       String;
  t_msg             s_tempMsg;
  bool              ignore_space = false;

  tempSS >> Token;
  s_tempMsg.command = Token;
  while (tempSS >> Token) {
    if (Token[0] == ':')
      ignore_space = true;
    if (ignore_space == false)
      s_tempMsg.paramVec.push_back(Token);
    else
      String += Token + " ";
  }
  //removes space from last token
  String = String.substr(0, String.size() - 1);
  if (ignore_space == true)
     s_tempMsg.paramVec.push_back(String);
  return s_tempMsg;
}


void Server::parsing_msg(std::string &Message, Client &client) {
	std::stringstream msg(Message);
	std::string       line;
  bool              ctrlD = false;

  if (Message.size() > 0 && Message[Message.size() - 1] != '\n') {
    client.addToBuffer(Message);
    ctrlD = true;
  }
  while (std::getline(msg, line, '\n')) {
    if (ctrlD == false) {
      client.addToBuffer(line);
      // if (DEBUG)
      //   std::cout << "CTRL - D TEST: " << client.getBuffer() << std::endl;
      this->_parMsg = tokenize_msg(client.getBuffer());
      // if (DEBUG) {
      //   std::cout << " .Com: " << this->_parMsg.command << std::endl;
      //   for (size_t i = 0; i < this->_parMsg.paramVec.size(); ++i) {
      //     std::cout << i << ".Tok: " << this->_parMsg.paramVec[i] << std::endl;
      //   }
      // }
      executeCommands(client, client.getBuffer());
      client.cleanBuffer();
    }
  }
}
