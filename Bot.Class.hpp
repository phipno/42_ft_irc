#pragma once

#include "Command.Class.hpp"

class Bot : public Command{

	private:
		std::vector<std::string> _db;
		Bot();

	public:
		Bot(Server &server, Client &client, std::string message);
		virtual ~Bot();

		virtual int executeCommand();
        std::string make_msg_ready(std::string yt_link);
};