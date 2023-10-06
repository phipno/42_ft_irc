#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
#include <sstream>

#include "Server.Class.hpp"
#include "Pass.Class.hpp"
#include "Nick.Class.hpp"
#include "User.Class.hpp"
#include "PrivMsg.Class.hpp"
#include "Join.Class.hpp"
#include "Invite.Class.hpp"



void  Server::executeCommands(Client &client, std::string Message) {
//switch case
  if (this->_parMsg.command == "PASS") {
    Command *command = new Pass(*this, client, Message);
    command->executeCommand();
    delete command;
    // this->pass(&this->_parMsg, client);
  } else if (this->_parMsg.command == "NICK") {
    Command *command = new Nick(*this, client, Message);
    command->executeCommand();
    delete command;
  } else if (this->_parMsg.command == "USER") {
    Command *command = new User(*this, client, Message);
    command->executeCommand();
    delete command;
      // this->user(&this->_parMsg, client);
  } else if (this->_parMsg.command == "JOIN") {
    Command *command = new Join(*this, client, Message);
    command->executeCommand();
    delete command;   
      // this->join(this->_parMsg, client);
  } else if (this->_parMsg.command == "OP") {
    // setOperator(Message);
  } else if (this->_parMsg.command == "PRIVMSG") {
    Command *command = new PrivMsg(*this, client, Message);
    command->executeCommand();
    delete command;
      // this->privmsg(&this->_parMsg, client);
  } else if (this->_parMsg.command == "KICK") {
    
  } else if (this->_parMsg.command == "TOPIC") {
      this->topic(&this->_parMsg, client);
  } else if (this->_parMsg.command == "MODE") {
    
  } else if (this->_parMsg.command == "INVITE") {
    Command *command = new Invite(*this, client, Message);
    command->executeCommand();
    delete command;
    // this->invite(&this->_parMsg, client);
  } else if (this->_parMsg.command == "LIST") {
      this->list(this->_parMsg, client);
  } else if (this->_parMsg.command == "CAP") {
      this->handshake(&this->_parMsg, client);
  } else if (this->_parMsg.command == "PING") {
    this->pong(&this->_parMsg, client);
  } else {
    throw(std::runtime_error("Command not found"));
  }
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
  try {
    this->_parMsg = tokenize_msg(Message);
    if (DEBUG) {
      std::cout << " .Com: " << this->_parMsg.command << std::endl;
      for (size_t i = 0; i < this->_parMsg.paramVec.size(); ++i) {
        std::cout << i << ".Tok: " << this->_parMsg.paramVec[i] << std::endl;
      }
    }
    executeCommands(client, Message);
    (void)client;
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
