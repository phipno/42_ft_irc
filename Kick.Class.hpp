#include "Command.Class.hpp"

#pragma once

class Kick : public Command{

    private:
        Kick();     

    public:
        Kick(Server &server, Client &client, std::string message);
        virtual ~Kick();

        virtual int executeCommand();
        int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);
};
