#include "Client.hpp"

Client::Client() : _userName("Unknown"), _nickName("Unknown") {}

Client::Client(std::string username, std::string nickname) : _userName(username),
															 _nickName(nickname) {}
															 