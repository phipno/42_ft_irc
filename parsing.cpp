#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <exception>

//prototypes that i need later for parsing
//log in, authenticate client and create new user
bool  logIn(std::string userName);
//nick, create nickname
bool setNickName(std::string newNickName);
//user, create username
bool setUserName(std::string newUserName);
//join, join a channel that is not invite-only
bool joinChannel(std::string channelToJoin);
//op, make a user operator on channel or whole server
bool setOperator(std::string nickName);
//prvmsg, write directly to another client or channel
bool privateMessAge(std::string writeTo, std::string textToBeSent);

//all commands with uppercase need to be implemented like in IRC_protocol 
//KICK, eject a client from channel
bool kick(std::vector<std::string> channelList, 
          std::vector<std::string> usersToKick, std::string commentToDisplay);

//INVITE, invite a client to a channel
bool invite(std::string nickName, std::string channelToBeInvited);

//TOPIC, change or view the hannel topic, if setTopic is NULL -> view topic, if setTopic is emptyString -> remove all Topics in channel
std::string topic(std::string channelToChange, std::string *setTopic);

//MODE, Change the channel mode
// +k set passwod, -k remove password
void  mode(std::string channelToChange, std::string modesFlag, std::string newPassword);
// +o set operator, -o remove operator
void  mode(std::string channelToChange, std::string modesFlag, 
           std::string nickNameToMakeOperator);
// +l set user limit to channel amount is user_limit, -l remove user limit
void  mode(std::string channelToChange, std::string modesFlag, int user_limit);
//+i set invite only channel, -i removce it
//+t set topic command to Operator only, -t topic command for regular users
void  mode(std::string channelToChange, std::string modesFlag);

extractCommand(std::string Message) {
  size_t      pos;
  std::string cmd;

  if (Message.[0] == ':') {

  }
  else {
    pos = std::find(Message.begin(), Message.end(), ' ');
    cmd = Message.substr(0, pos);
    Message.erase(0, pos);
  }
  std::cout << cmd << std::endl;
  executeCommands(cmd);
}

void  executeMessage(std::string cmd, std::string message) {
  switch (cmd){
    case "LOGIN":

    case "NICK":

    case "USER":

    case "JOIN":

    case "OP":

    case "PRVMSG":

    case "KICK":

    case "TOPIC":

    case "MODE":

    case "INVITE":

  }
}

int parsing(std::string Message /*, Client &User*/) {
  try {
    extractCommand(Message);

  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}