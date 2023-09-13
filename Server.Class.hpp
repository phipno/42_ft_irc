#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_CONNECTIONS 100

class Server{

    private:
        int                 _port;
        std::string         _password;

        int                 _serverSocket;
        struct sockaddr_in  _serverAddr;
        std::vector<int>    _clients; //to be changed to vector containing objects of class Client
        bool                _run;
        pthread_mutex_t     _clientMutex;
        Server();

    public:
        ~Server();
        Server(int port, std::string password);
        Server (Server const &src);
        Server &operator= (Server const &src);

        int setup(); // to be put private later
};