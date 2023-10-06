#include "Command.Class.hpp"

#pragma once

class Join : public Command{

    private:
        Join();     

    public:
        Join(Server &server, Client &client, std::string message);
        virtual ~Join();

        virtual int executeCommand();
        int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);
        
        std::vector<std::string> parse_join_kick(std::string commaToken);
};
