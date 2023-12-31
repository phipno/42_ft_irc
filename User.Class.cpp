#include "User.Class.hpp"

/*  Command: USER
   Parameters: <username> <hostname> <servername> <realname>

   The USER message is used at the beginning of connection to specify
   the username, hostname, servername and realname of a new user.  It is
   also used in communication between servers to indicate new user
   arriving on IRC, since only after both USER and NICK have been
   received from a client does a user become registered.

   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

TO-DO: 4 Names must be set for Username
*/

User::User(Server &server, Client &client, std::string message) : Command(server, client , message){}

User::~User() {};

/* ---------------------- METHODS --------------------------------------------*/

int User::executeCommand(){

    Command::tokenizeMsg();
    if (Command::checkRegistrationStatus())
        return 1;
    if (checkEmptyParamter()) // to be changed to class specific function (4 params)
        return 1;
    
    if (this->_paramVec[0].length() <= USERLEN)
        this->_client->setFullName(this->_paramVec[0]);
    else {
        std::string name = this->_paramVec[0].substr(0,9);
        this->_client->setFullName(name);
    }
    
    // if client typed in nickname, he will be welcomed after username
    if (this->_client->getStatus() == NICKNAME) {
        Command::numReply(RPL_WELCOME(this->_server->getHostname(), this->_client->getNickName(), this->_client->getUserName()));
        Command::numReply(RPL_YOURHOST(this->_server->getHostname(), this->_client->getNickName()));
        this->_client->registerClient(WELCOMED);
        return 0;
    }
    else if (this->_client->getStatus() == WELCOMED) {
        ; //do nothing when trying to change the username
    }
    else {
        this->_client->setFullName(this->_paramVec[0]);
        this->_client->registerClient(USERNAME);
    }
    return 0;
}

/* command requires 4 parameters */
int User::checkEmptyParamter(){
    // std::cout << MAGENTA << "EMPTYPARAM\n" << RESET;
    if (this->_paramVec.empty() || this->_paramVec.size() < 3){
        Command::numReply(ERR_NEEDMOREPARAMS(this->_server->getHostname(), this->_client->getNickName(), this->_command));
        return (1);
    }
    return (0);
}