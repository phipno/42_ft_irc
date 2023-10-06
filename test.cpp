#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h> // for fcntl

#include <poll.h> // for poll

const int PORT = 6667;
const int MAX_CONNECTIONS = 10;
const int MAX_EVENTS = 100;

std::vector<int> clientSockets;
pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;

void* handleClient(void* clientSocketPtr) {
    int clientSocket = *((int*)clientSocketPtr);
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            pthread_mutex_lock(&clientMutex);
            std::vector<int>::iterator it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
            if (it != clientSockets.end()) {
                clientSockets.erase(it);
                close(clientSocket);
                std::cout << "Client disconnected" << std::endl;
            }
            pthread_mutex_unlock(&clientMutex);
            break;
        }
        std::string message(buffer);
        std::cout << "Received: " << message;
        pthread_mutex_lock(&clientMutex);
        for (size_t i = 0; i < clientSockets.size(); ++i) {
            send(clientSockets[i], buffer, bytesRead, 0);
        }
        pthread_mutex_unlock(&clientMutex);
    }
    pthread_exit(NULL);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }

    // Set serverSocket to non-blocking mode
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    std::cout << "Server listening on port " << PORT << std::endl;

    struct pollfd pollfds[MAX_EVENTS];
    pollfds[0].fd = serverSocket;
    pollfds[0].events = POLLIN;

    int nfds = 1;

    while (true) {
        int pollResult = poll(pollfds, nfds, -1); // Wait indefinitely

        if (pollResult == -1) {
            perror("poll");
            continue;
        }

        if (pollfds[0].revents & POLLIN) {
            // New connection
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int* clientSocketPtr = new int;
            *clientSocketPtr = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
            if (*clientSocketPtr == -1) {
                std::cerr << "Error accepting client connection" << std::endl;
                continue;
            }
            pthread_mutex_lock(&clientMutex);
            clientSockets.push_back(*clientSocketPtr);
            std::cout << "Client connected" << std::endl;
            pthread_mutex_unlock(&clientMutex);
            
            // Set the new client socket to non-blocking mode
            flags = fcntl(*clientSocketPtr, F_GETFL, 0);
            fcntl(*clientSocketPtr, F_SETFL, flags | O_NONBLOCK);

            // Add the new client socket to pollfds
            pollfds[nfds].fd = *clientSocketPtr;
            pollfds[nfds].events = POLLIN;
            nfds++;
        }

        for (int i = 1; i < nfds; ++i) {
            if (pollfds[i].revents & POLLIN) {
                // Handle data from clients
                int clientSocket = pollfds[i].fd;
                // Handle the client data as before in the handleClient function
                // ...
            }
        }
    }

    close(serverSocket);
    return 0;
}



// #include <iostream>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <cstring>
// #include <cstdlib>
// #include <cstdio>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <pthread.h>

// const int PORT = 6667;
// const int MAX_CONNECTIONS = 10;

// std::vector<int> clientSockets;
// pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;

// void* handleClient(void* clientSocketPtr) {
//     int clientSocket = *((int*)clientSocketPtr);
//     char buffer[1024];
//     while (true) {
//         memset(buffer, 0, sizeof(buffer));
//         int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (bytesRead <= 0) {
//             pthread_mutex_lock(&clientMutex);
//             std::vector<int>::iterator it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
//             if (it != clientSockets.end()) {
//                 clientSockets.erase(it);
//                 close(clientSocket);
//                 std::cout << "Client disconnected" << std::endl;
//             }
//             pthread_mutex_unlock(&clientMutex);
//             break;
//         }
//         std::string message(buffer);
//         std::cout << "Received: " << message;
//         pthread_mutex_lock(&clientMutex);
//         for (size_t i = 0; i < clientSockets.size(); ++i) {
//             send(clientSockets[i], buffer, bytesRead, 0);
//         }
//         pthread_mutex_unlock(&clientMutex);
//     }
//     pthread_exit(NULL);
// }

// int main() {
//     // create socket
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Error creating socket" << std::endl;
//         return 1;
//     }
//     else
//         std::cout << "Server socket successfully created" << std::endl;

//     /* Socket address, internet style.
//     struct sockaddr_in {
// 	    __uint8_t       sin_len;
// 	    sa_family_t     sin_family;
// 	    in_port_t       sin_port;
// 	    struct  in_addr sin_addr;
// 	    char            sin_zero[8];
//     };*/
//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(PORT);
//     // serverAddr.sin_addr.s_addr = INADDR_ANY;
//     serverAddr.sin_addr.s_addr = htonl(2130706433);

//     // bind socket
//     if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
//         std::cerr << "Error binding socket" << std::endl;
//         return 1;
//     }
//      else
//         std::cout << "Server socket successfully binded" << std::endl;


//     if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
//         std::cerr << "Error listening on socket" << std::endl;
//         return 1;
//     }

//     std::cout << "Server listening on port " << PORT << std::endl;

//     while (true) {
//         sockaddr_in clientAddr;
//         socklen_t clientAddrLen = sizeof(clientAddr);
//         int* clientSocketPtr = new int;
//         *clientSocketPtr = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
//         if (*clientSocketPtr == -1) {
//             std::cerr << "Error accepting client connection" << std::endl;
//             continue;
//         }
//         pthread_mutex_lock(&clientMutex);
//         clientSockets.push_back(*clientSocketPtr);
//         std::cout << "Client connected" << std::endl;
//         pthread_mutex_unlock(&clientMutex);
//         pthread_t clientThread;
//         if (pthread_create(&clientThread, NULL, handleClient, clientSocketPtr) != 0) {
//             std::cerr << "Error creating client thread" << std::endl;
//             delete clientSocketPtr;
//             continue;
//         }
//     }

//     close(serverSocket);
//     return 0;
// }
