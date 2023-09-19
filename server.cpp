// #include <sys/socket.h> // For socket functions
// #include <netinet/in.h> // For sockaddr_in
// #include <cstdlib> // For exit() and EXIT_FAILURE
// #include <iostream> // For cout
// #include <unistd.h> // For read


// //https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// int main() {
//   // Create a socket (IPv4, TCP)
//   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//   if (sockfd == -1) {
//     std::cout << "Failed to create socket. errno: " << errno << std::endl;
//     exit(EXIT_FAILURE);
//   }

//   // Listen to port 9999 on any address
//   sockaddr_in sockaddr;
//   sockaddr.sin_family = AF_INET;
//   sockaddr.sin_addr.s_addr = INADDR_ANY;
//   sockaddr.sin_port = htons(9999); // htons is necessary to convert a number to
//                                    // network byte order
//   if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
//     std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
//     exit(EXIT_FAILURE);
//   }

//   // Start listening. Hold at most 10 connections in the queue
//   if (listen(sockfd, 10) < 0) {
//     std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
//     exit(EXIT_FAILURE);
//   }

//   // Grab a connection from the queue
//   auto addrlen = sizeof(sockaddr);
//   int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
//   if (connection < 0) {
//     std::cout << "Failed to grab connection. errno: " << errno << std::endl;
//     exit(EXIT_FAILURE);
//   }

//   // Read from the connection
//   char buffer[100];
//   auto bytesRead = read(connection, buffer, 100);
//   std::cout << "The message was: " << buffer;

//   // Send a message to the connection
//   std::string response = "Good talking to you\n";
//   send(connection, response.c_str(), response.size(), 0);

//   // Close the connections
//   close(connection);
//   close(sockfd);
// }


//https://github.com/weboutin/simple-socket-server/blob/master/client.c
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <poll.h>

#define PORT 9991
#define SIZE 1024
#define MAX_CLIENTS 10

int creat_socket()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //binds the socket to the address struct, which holds the port and 
    int bindResult = bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (bindResult == -1)
    {
        perror("bindResult");
    }

    int listenResult = listen(server_socket, 5);
    if (listenResult == -1)
    {
        perror("listenResult");
    }
    printf("server start\n");

	printf("server socket: %d\n", server_socket);
    return server_socket;
}

int wait_client(int server_socket)
{
    struct pollfd pollfds[MAX_CLIENTS + 1];
    pollfds[0].fd = server_socket;
    pollfds[0].events = POLLIN | POLLPRI;
    int useClient = 0;

    while (1)
    {
        // printf("useClient => %d\n", useClient);
        int pollResult = poll(pollfds, useClient + 1, 5000);
        if (pollResult > 0)
        {
            if (pollfds[0].revents & POLLIN)
            {
                struct sockaddr_in cliaddr;
                int addrlen = sizeof(cliaddr);
                int client_socket = accept(server_socket, (struct sockaddr *)&cliaddr, (socklen_t*)&addrlen);
                printf("accept success %s\n", inet_ntoa(cliaddr.sin_addr));
                for (int i = 1; i < MAX_CLIENTS; i++)
                {
                    if (pollfds[i].fd == 0)
                    {

                        pollfds[i].fd = client_socket;
                        pollfds[i].events = POLLIN | POLLPRI;
                        useClient++;
                        break;
                    }
                }
            }
            for (int i = 1; i < MAX_CLIENTS; i++)
            {
                if (pollfds[i].fd > 0 && pollfds[i].revents & POLLIN)
                {
                    char buf[SIZE];
                    int bufSize = read(pollfds[i].fd, buf, SIZE - 1);
                    if (bufSize == -1)
                    {
                        pollfds[i].fd = 0;
                        pollfds[i].events = 0;
                        pollfds[i].revents = 0;
                        useClient--;
                    }
                    else if (bufSize == 0)
                    {
                        pollfds[i].fd = 0;
                        pollfds[i].events = 0;
                        pollfds[i].revents = 0;
                        useClient--;
                    }
                    else
                    {

                        buf[bufSize] = '\0';
                        printf("From client: %s\n", buf);
                    }
                }
            }
        }
    }
}

int main()
{
    int server_socket = creat_socket();

    int client_socket = wait_client(server_socket);

    printf("server end\n");

    close(client_socket);
    close(server_socket);

    return 0;
}
