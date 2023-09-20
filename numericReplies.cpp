#include "Server.Class.hpp"
#include "Client.Class.hpp"


void Server::numReply(int errorCode, t_msg *message, Client client){

    (void) client;
    switch(errorCode){
        case ERR_NONICKNAMEGIVEN:
            std::cout << "ERR_NONICKNAMEGIVEN: No nickname given\n";
            break;
        case ERR_ALREADYREGISTRED:
            std::cout << "ERR_ALREADYREGISTRED:Unauthorized command (already registered)\n";
            break;
        case ERR_NEEDMOREPARAMS:
            std::cout << message->command << " : Not enough parameters";
            break;
        default:
           std::cout << "unkown error\n";
    }
}

