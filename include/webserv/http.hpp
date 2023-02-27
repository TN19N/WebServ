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

// Connection Handler **********************************************************************************************
void    acceptConnection(int listenFd, std::vector<pollfd>& fds, std::vector<Client*>& clients);

// Request Handler **************************************************************************************************
void    requestHandler(Client* client, const Context* const configuration, std::vector<const Client*>& clientsToRemove);
Request* request_parser(const std::string &_buffer);

// Response Handler *************************************************************************************************
const bool  sendResponse(const Client* client, const int& statusCode, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>());
void    responseHandler(Client* client, Request* request, const Context* const configuration);
const Context*  blockMatchAlgorithm(Client* client, const Request* request, const Context* const configuration);

// Status Code Handler ***********************************************************************************************
const std::string   getStatusCodeMessage(int statusCode);

// Cgi Handler *******************************************************************************************************
Client* runCgi(const Context* location, const Client* client);

// Tools *************************************************************************************************************
const std::string   getHttpDate(void);

} // namespace HTTP
# endif