#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>

#include <string>
#include <iostream>

# define MAXDATASIZE 100

class Client
{
private:
    std::string         __host_;
    int                 __port_;
    int                 __ipType_;
    int                 __sockType_;
    int                 __sockfd_;
    std::string         __name_;

    class ClientCrash : public std::exception
    {
    private:
        std::string __msg_;
    public:
        ClientCrash(const std::string& msg) : __msg_(msg) {};
        const char* what() const _NOEXCEPT {
            return __msg_.c_str();
        }
        ~ClientCrash() _NOEXCEPT {};
    };
public:
    Client(const std::string &host, const int& port, const int& sockType, const int& ipType, const std::string &name)
        : __host_(host),
          __port_(port),
          __sockType_(sockType),
          __ipType_(__ipType_),
          __name_(name),
          __sockfd_(-1)
    {
        struct addrinfo     hints, *addrInfo;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = __ipType_;
        hints.ai_socktype = __sockType_;

        {
            int rv;
            if ((rv = getaddrinfo(__host_.c_str(), std::to_string(__port_).c_str(), &hints, &addrInfo)) != 0)
                throw(ClientCrash("Client[" + __name_ + "]: getaddrinfo(): " + gai_strerror(rv)));
        }

        for (struct addrinfo *__p = addrInfo; __p; __p = __p->ai_next)
        {
            if ((__sockfd_ = socket(__p->ai_family, __p->ai_socktype, __p->ai_protocol)) == -1)
            {
                perror(("Client[" + __name_ + "]: socket()").c_str());
                continue;
            }
            if (connect(__sockfd_, __p->ai_addr, __p->ai_addrlen) == -1)
            {
                close(__sockfd_); __sockfd_ = -1;
                perror(("Client[" + __name_ + "]: connect()").c_str());
                continue;
            }
            break;
        }

        if (addrInfo)
            freeaddrinfo(addrInfo);
        
        if (__sockfd_ == -1)
            throw(ClientCrash("Client[" + __name_ + "]: failed to connect()"));
    };

    void startConnecting(void)
    {
        std::string request = __name_;
        char        buffer[MAXDATASIZE];
        int         numberOfBytes;
        bool        sendName = true;

        while (true)
        {
            if (!sendName)
            {
                std::cout << ">\n";
                std::getline(std::cin, request);
                if (std::cin.eof())
                    break;
            }
            
            while (request.size())
            {
                if ((numberOfBytes = send(__sockfd_, request.c_str(), request.size(), 0)) == -1)
                {
                    close(__sockfd_);
                    throw(ClientCrash("Client[" + __name_ + "]: send()" + strerror(errno)));
                }
                request.erase(0, numberOfBytes);
            }
            if (sendName)
                sendName = false;
        }

        close(__sockfd_);
        std::cout << "Client[" + __name_ + "]: connection closed" << std::endl;
    }
};

#endif