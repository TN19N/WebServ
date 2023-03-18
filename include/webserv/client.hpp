# ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstddef>
# include <sys/socket.h>
# include <string>
# include <poll.h>

#include "defines.hpp"
# include "request.hpp"
# include "response.hpp"
#include "context.hpp"

class Client {
    private:
        const int                       socketFd;
        const struct sockaddr_storage   clientAddr;
        const struct sockaddr_storage   peerAddr;
        int                             pipeFd[2];

        Request*                        request;
        Response*                       response;

        Client*                         cgiToClient;
        Client*                         clientToCgi;

        std::string                     buffer;

        int                             state;
    public:
        Client(const int* fd, const struct sockaddr_storage& clientAddr, const struct sockaddr_storage& peerAddr, Client* cgiToClient = nullptr);

        int                             getSocketFd() const;
        const struct sockaddr_storage&  getClientAddr() const;
        const struct sockaddr_storage&  getPeerAddr() const;

        const int*                      getPipeFd() const;
        int*                            getPipeFd();

        const Request*                  getRequest() const;
        Request*                        getRequest();

        const Response*                 getResponse() const;
        Response*                       getResponse();

        const Client*                   getCgiToClient() const;
        const Client*                   getClientToCgi() const;

        const std::string&              getBuffer() const;
        std::string&                    getBuffer();

        int                             getState() const;

        int                             getFdOf(const int index) const;

        void                            setClientToCgi(Client* clientToCgi);
        void                            setRequest(Request* request);
        void                            setResponse(Response* response);
        void                            setState(const int &state);

        bool                            isCgi() const;
        void                            switchState();

        ~Client();
};

# endif