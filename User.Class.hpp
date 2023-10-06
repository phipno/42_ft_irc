#include "Command.Class.hpp"

#pragma once

class User : public Command{

	private:
		User();		

	public:
		User(Server &server, Client &client, std::string message);
		virtual ~User();

		virtual int executeCommand();
        int checkEmptyParamter();
};