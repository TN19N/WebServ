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
    void    acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients);
    // request handler
    void    requestHandler(Client* client, std::vector<const Client*>& clientsToRemove);
    // response handler
    const bool  sendResponse(const Client* client, const int& statusCode, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>());
    void    responseHandler(Client* client, Request* request, const Context* const configuration);
    const Context*  blockMatchAlgorithm(Client* client, Request* request, const Context* const configuration);
    // status code
    const std::string   getStatusCodeMessage(int statusCode);
    // tools
    const std::string   getHttpDate(void);
}
# endif