# ifndef HTTP_HPP
# define HTTP_HPP

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>

# include "webserv/client.hpp"

namespace HTTP {

// * Connection Handler ***********************************************************************
void closeConnection(std::vector<pollfd>& fds, const int index, std::vector<Client*>& clients);
void acceptConnection(const int serverFd, std::vector<Client*>& clients);

// * Status Code Handler ************************************
const std::string getStatusCodeMessage(const int statusCode);

// * Response Handler ******************
const bool sendResponse(Client* client);

// * Request Handler ***************
void requestHandler(Client* client);

// * Tools **********************************************************************************
Client* getClientWithFd(const int fd, const std::vector<Client*>& clients);
const std::string getDefaultErrorPage(const int statusCode);
const std::string getHttpDate();

} // namespace HTTP

# endif