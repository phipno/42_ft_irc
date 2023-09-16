#pragma once

class Channel;

typedef struct s_info {
	
	bool _operator;
	bool _kicked;

} t_info;

class Client {
	
	protected:
		std::string _username;
		std::string _fullname;
	
	public:
		
		std::map<std::string, t_info> _channels; //list of channels that the client is in plus additional info, if he is operatr or got kicked
		void add_channel(std::string name, t_info infos);
		
		std::string prv_message(std::string message, class Client &cli); //sends a private message to an user
		std::string message(std::string message, class Channel &chnl); //sends a message to every user inside the channel


};