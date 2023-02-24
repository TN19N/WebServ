# ifndef HTTP_HPP
# define HTTP_HPP

# include <vector>
# include <poll.h>
# include <string>

# include "webserv/client.hpp"
# include "webserv/request.hpp"
# include "webserv/context.hpp"

# define CRLF "\r\n"

namespace HTTP {
    // accept connection
    void    acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients);
    // request handler
    void    requestHandler(Client* client, std::vector<int>& fdsToClose);
    // response handler
    const bool  sendResponse(const Client* client, const int& statusCode, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>());
    void        responseHandler(Client* client, Request* request, const Context* const configuration);
    // block match algorithm
    const Context*  blockMatchAlgorithm(Client* client, Request* request, const Context* const configuration);
    // status code handler
    void                sendErrorStatusCode(int clientFd, int statusCode);
    const std::string   getStatusCodeMessage(int statusCode);
    // tools
    const std::string getHttpDate(void);
}
# endif