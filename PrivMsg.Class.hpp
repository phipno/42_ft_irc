#include "Command.Class.hpp"

#pragma once

class PrivMsg : public Command{

    private:
        PrivMsg();     

    public:
        PrivMsg(Server &server, Client &client, std::string message);
        virtual ~PrivMsg();

        virtual int executeCommand();
        int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);
};
