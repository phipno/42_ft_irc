#include "Server.Class.hpp"
#include "Client.Class.hpp"


std::string Server::numReply(int errorCode, t_msg *message, Client client){

    std::string reply;

    (void) client;
    switch(errorCode){
        case ERR_NONICKNAMEGIVEN:
            reply =  ": " + this->_hostname + " 431 " + client.getNickName() + ":No nickname given\n";
            break;
        case ERR_ALREADYREGISTRED:
            reply = ": " + this->_hostname + "462 " + client.getNickName() + ":You may not reregister\n";
            break;
        case ERR_NEEDMOREPARAMS:
            reply = message->command + ": Not enough parameters";
            break;
        default:
           std::cout << "unkown error\n";
    }
    return NULL;
}

