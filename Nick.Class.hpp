#include "Command.Class.hpp"

#pragma once

class Nick : public Command{

	private:
		Nick();		

	public:
		Nick(Server &server, Client &client, std::string message);
		virtual ~Nick();

		virtual int executeCommand();
		int checkEmptyParamter();
};