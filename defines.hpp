//protocol errors
#define MAX_CONNECTIONS 100
#define MAX_EVENTS 100
#define RPL_WELCOME 001
#define RPL_YOURHOST 002
#define RPL_CREATED 003
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_ALREADYREGISTRED 462
#define ERR_NEEDMOREPARAMS 461
#define ERR_NICKNAMEINUSE 433
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412

//custom errors
#define ERR_PING 005

//Ping/Pong Message
#define PONG 1234567890
#define ERR_NOORIGIN 409

//debugflags
#define VERBOSE 0 //used for constructors and function calls
#define LOOP 0 //used for printing when inside a loop
#define DEBUG 1 // generic print flag