#include "Command.Class.hpp"

#pragma once

class Pass : public Command{

	private:
		Pass();		

	public:
		Pass(Server &server, Client &client, std::string message);
		virtual ~Pass();

		virtual int executeCommand();
		int checkEmptyParamter();
		int checkRegistrationStatus();
};