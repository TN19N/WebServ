#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <sys/wait.h>

#include <string>
#include <exception>
#include <iostream>

# define MAXDATASIZE 100
# define BACKLOG 30

class Server
{
private:
    int                 __port_;
    int                 __sockType_;
    int                 __ipType_;
    int                 __sockfd_;

    class ServerDown : public std::exception
    {
    private:
        std::string __msg_;
    public:
        ServerDown(const std::string& msg) : __msg_(msg) {};
        const char* what() const _NOEXCEPT {
            return __msg_.c_str();
        }
        ~ServerDown() _NOEXCEPT {};
    };
public:
    Server(const int& port, const int& sockType, const int& ipType)
        : __port_(port),
          __sockType_(sockType),
          __ipType_(ipType),
          __sockfd_(-1)
    {
        struct addrinfo     hints, *addrInfo;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = __ipType_;
        hints.ai_socktype = __sockType_;

        {
            int rv;
            if ((rv = getaddrinfo(NULL, std::to_string(__port_).c_str(), &hints, &addrInfo)) != 0)
                throw(ServerDown(std::string("[Server]: getaddrinfo(): ") + gai_strerror(rv)));
        }

        for (struct addrinfo *__p = addrInfo; __p; __p = __p->ai_next)
        {
            if ((__sockfd_ = socket(__p->ai_family, __p->ai_socktype, __p->ai_protocol)) == -1)
            {
                perror("[Server]: socket()");
                continue;
            }
            if (bind(__sockfd_, __p->ai_addr, __p->ai_addrlen) == -1)
            {
                close(__sockfd_); __sockfd_ = -1;
                perror("[Server]: bind()");
                continue;
            }
            break;
        }

        if (addrInfo)
            freeaddrinfo(addrInfo);
        
        if (__sockfd_ == -1)
            throw(ServerDown("[Server]: failed to bind"));

        if (listen(__sockfd_, BACKLOG) == -1)
            throw(ServerDown("[Server]: listen failed"));

        struct sigaction    sa;

        sa.sa_handler = Server::sigHandler;
        sa.sa_flags = SA_RESTART;

        if (sigaction(SIGCHLD, &sa, NULL) == -1)
            throw(ServerDown("[Server]: sigaction() failed"));
    };

    static void sigHandler(int sig) {
        (void)sig;
        int saveErrno = errno;
        while (waitpid(-1, NULL, WNOHANG) > 0);
        errno = saveErrno;
    }

    void startListening(void)
    {
        std::cout << "[Server]: start listening on port " << __port_ << std::endl;
        while (true)
        {
            struct sockaddr_storage client;
            socklen_t sockaddr_len = sizeof(client);
            std::string clientName;
            
            int newfd = accept(__sockfd_, (struct sockaddr *)&client, &sockaddr_len);
            if (newfd == -1)
            {
                perror("[Server]: accept()");
                continue;
            }

            if (!fork())
            {
                close(__sockfd_);
                
                char buffer[MAXDATASIZE];
                std::string request;
                std::string respones;
                int numberOfBytes = 0; 

                while (true)
                {
                    if ((numberOfBytes = recv(newfd, buffer, MAXDATASIZE - 1, 0)) == -1)
                    {
                        perror(("[Server][" + clientName + "]: recv()").c_str());
                        exit(1);
                    }

                    if (numberOfBytes == 0)
                    {
                        close(newfd);
                        std::cout << "[Server][" + clientName + "]: connection closed from client side" << std::endl;
                        exit(0);
                    }

                    buffer[numberOfBytes] = '\0';
                    
                    request += buffer;
                    if (request.find(EOF) != std::string::npos)
                    {
                        if (!clientName.size())
                            clientName = request.substr(0, request.find(EOF));
                        std::cout << "[Server][" + clientName + "][request]: " << request.substr(0, request.find(EOF)) << std::endl;
                        respones = "server received the request (states: 200OK)";
                        respones += EOF;
                        while (respones.size())
                        {
                            if ((numberOfBytes = send(newfd, respones.c_str(), respones.size(), 0)) == -1)
                            {
                                close(newfd);
                                perror(("[Server][" + clientName + "]: send()").c_str());
                                exit(1);
                            }
                            respones.erase(0, numberOfBytes);
                        }
                        request.erase(0, request.find(EOF) + 1);
                    }
                }
            }
            close(newfd);
        }
    }
};

#endif