# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstddef>
# include <sys/socket.h>
# include <string>

# include "webserv/request.hpp"
# include "webserv/response.hpp"

# define USER_CLIENT 0
# define CGI_CLIENT  1

# define READ_END  0
# define WRITE_END 1

class Client {
    private:
        const int                       fd;
        const struct sockaddr_storage   clientAddr;
        const struct sockaddr*          peerAddr;
        int                             pipeFd[2];

        Request*                        request;
        Response*                       response;

        bool                            type;
        Client*                         cgiToClient;
        Client*                         clientToCgi;

        std::string                     buffer;
    public:
        Client(const int* fd, const struct sockaddr_storage& clientAddr, const struct sockaddr* peerAddr, Client* cgiToClient = nullptr);

        const int&                      getFd() const;
        const struct sockaddr_storage&  getClientAddr() const;
        const struct sockaddr*          getPeerAddr() const;
        const int*                      getPipeFd() const;

        const Request*                  getRequest() const;
        const Response*                 getResponse() const;
        Request*                        getRequest();
        Response*                       getResponse();

        bool                            getType() const;
        const Client*                   getCgiToClient() const;
        const Client*                   getClientToCgi() const;

        const std::string&              getBuffer() const;

        void                            setClientToCgi(Client* clientToCgi);

        ~Client();
};

# endif