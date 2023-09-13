#include "Server.Class.hpp"

/* ---------------- CANONICAL FORM ---------------------------*/
Server::Server() : _port(0), _password("no_pw"){};
Server::Server(int port, std::string password) : _port(port), _password(password){
    if (pthread_mutex_init(&this->_clientMutex, NULL) != 0) {
            std::cerr << "Mutex initialization failed" << std::endl;
        }
};
Server::~Server(){
    pthread_mutex_destroy(&this->_clientMutex);
};
Server::Server (Server const &src){};
Server &Server::operator= (Server const &src){
    this->_serverSocket = src._serverSocket;
    this->_port = src._port;
    this->_password = src._password;
    return (*this);
};


/* ---------------- PRIVATE METHODS ---------------------------*/

int Server::setup(){
    // create serversocket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1){
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // bind serversocket, using internet style (struct sockaddr_in), Apple specific?
    this->_serverAddr.sin_family = AF_INET;
    this->_serverAddr.sin_port = htons(this->_port);
    this->_serverAddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
    // serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(_serverSocket, reinterpret_cast<struct sockaddr*>(&this->_serverAddr), sizeof(this->_serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1){
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }

    // set serversocket to non-blocking mode
    fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK);

    this->_run = true;
    std::cout << "Server listening on port " << this->_port << std::endl;
    return 0;
};