#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>
#include <sstream>

#include "Server.Class.hpp"



void  Server::executeCommands(Client &client) {

  if (this->_parMsg.command == "PASS") {
    this->pass(&this->_parMsg, client);
  } else if (this->_parMsg.command == "NICK") {
      this->nick(&this->_parMsg, client);
  } else if (this->_parMsg.command == "USER") {
      this->user(&this->_parMsg, client);
  } else if (this->_parMsg.command == "JOIN") {
      this->join(this->_parMsg, client);
  } else if (this->_parMsg.command == "OP") {
    // setOperator(Message);
  } else if (this->_parMsg.command == "PRIVMSG") {
      this->privmsg(&this->_parMsg, client);
  } else if (this->_parMsg.command == "KICK") {
    
  } else if (this->_parMsg.command == "TOPIC") {
   
  } else if (this->_parMsg.command == "MODE") {
    
  } else if (this->_parMsg.command == "INVITE") {
   
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
    executeCommands(client);
    (void)client;
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
