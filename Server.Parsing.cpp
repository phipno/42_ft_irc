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
      this->join_channel("Testchannel", client);
  } else if (this->_parMsg.command == "OP") {
    // setOperator(Message);
  } else if (this->_parMsg.command == "PRIVMSG") {
    
  } else if (this->_parMsg.command == "KICK") {
    
  } else if (this->_parMsg.command == "TOPIC") {
   
  } else if (this->_parMsg.command == "MODE") {
    
  } else if (this->_parMsg.command == "INVITE") {
   
  } else if (this->_parMsg.command == "PONG") {
    this->pong(&this->_parMsg, client);
  } 
  else {
    throw(std::runtime_error("Command not found"));
  }
}

t_msg tokenize_msg(std::string Message) {
  std::stringstream tempSS(Message);
  std::string       tempToken;
  t_msg             s_tempMsg;

  tempSS >> tempToken;
  s_tempMsg.command = tempToken;
  while (tempSS >> tempToken) {
    s_tempMsg.paramVec.push_back(tempToken);
  }
  //probably dont need this
  if (s_tempMsg.paramVec.size() > 1)
    s_tempMsg.param = s_tempMsg.paramVec[1];
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

//prototypes that i need later for parsing
//log in, authenticate client and create new user
void  logIn(std::string userName);
//nick, create nickname
void setNickName(std::string newNickName);
//user, create username
void setUserName(std::string newUserName);
//join, join a channel that is not invite-only
void joinChannel(std::string channelToJoin);
//op, make a user operator on channel or whole server
void setOperator(std::string nickName);
//prvmsg, write directly to another client or channel
void privateMessAge(std::string writeTo, std::string textToBeSent);

//all commands with uppercase need to be implemented like in IRC_protocol 
//KICK, eject a client from channel
void kick(std::vector<std::string> channelList, 
          std::vector<std::string> usersToKick, std::string commentToDisplay);

//INVITE, invite a client to a channel
void invite(std::string nickName, std::string channelToBeInvited);

//TOPIC, change or view the hannel topic, if setTopic is NULL -> view topic, if setTopic is emptyString -> remove all Topics in channel
std::string topic(std::string channelToChange);
std::string topic(std::string channelToChange, std::string setTopic);

//MODE, Change the channel mode
// +k set passwod, -k remove password
void  mode(std::string channelToChange, std::string modesFlag, std::string Message);
// +o set operator, -o remove operator
// +l set user limit to channel amount is user_limit, -l remove user limit
//+i set invite only channel, -i removce it
//+t set topic command to Operator only, -t topic command for regular users
