#include "Client.hpp"

Client::Client() : _username("Unknown"), _fullname("Unknown") {}

Client::Client(std::string username, std::string fullname) : _username(username),
															 _fullname(fullname) {}

void Client::add_channel(std::string name, t_info infos) {

    _channels[name] = infos;
}
