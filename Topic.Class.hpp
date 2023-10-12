#include "Command.Class.hpp"

#pragma once

class Topic : public Command{

	private:
		Topic();		

	public:
		Topic(Server &server, Client &client, std::string message);
		virtual ~Topic();

		virtual int executeCommand();
		int checkEmptyParamter();
        std::string make_msg_ready(size_t channelnumber, std::string topic_message);
		bool is_empty_string(std::string token);
};
