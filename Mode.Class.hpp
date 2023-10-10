#include "Command.Class.hpp"

#pragma once

class Mode : public Command{

    private:
        Mode();     
        int topic_invite_restriction(std::vector<std::string> params, int pos);
        bool is_in_modes(std::string param);
        int key_mode(std::vector<std::string> params, int pos);
        bool valid_passphrase(std::string param);
        int user_limit(std::vector<std::string> params, int pos);
        int valid_number(std::string param, int channelindex);
        int operator_mode(std::vector<std::string> params, int pos);
        int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);

    public:
        Mode(Server &server, Client &client, std::string message);
        virtual ~Mode();

        virtual int executeCommand();

};