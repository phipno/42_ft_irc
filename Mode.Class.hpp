#include "Command.Class.hpp"

#pragma once

class Mode : public Command{

    private:
        Mode();     

    public:
        Mode(Server &server, Client &client, std::string message);
        virtual ~Mode();

        virtual int executeCommand();
        int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);

        int topic_invite_restriction(std::vector<std::string> params, int pos, class Client &client);
        bool is_in_modes(std::string param);
};