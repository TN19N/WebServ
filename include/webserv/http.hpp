# ifndef HTTP_HPP
# define HTTP_HPP

# include <vector>
# include <poll.h>
# include <string>

# include "client.hpp"
# include "request.hpp"
# include "context.hpp"

# define CRLF "\r\n"

namespace HTTP {
    void    acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients);
    // request handler
    void    requestHandler(Client* client, const Context* const configuration, std::vector<const Client*>& clientsToRemove);
    Request* request_parser(const std::string &_buffer);
    // response handler
    const bool  sendResponse(const Client* client, const int& statusCode, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>());
    void    responseHandler(Client* client, Request* request, const Context* const configuration);
    const Context*  blockMatchAlgorithm(Client* client, const Request* request, const Context* const configuration);
    // status code
    const std::string   getStatusCodeMessage(int statusCode);
    // cgi handler
    Client* runCgi(const Context* location, const Client* client);
    // tools
    const std::string   getHttpDate(void);
}
# endif