# ifndef HTTP_HPP
# define HTTP_HPP

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>

# include "webserv/client.hpp"

namespace HTTP {

// * Connection Handler ********************************************************************************************************************
void acceptConnection(const int serverFd, std::vector<pollfd>& fds, std::vector<Client*>& clients, const struct sockaddr* serverAddr);
void closeConnection(std::vector<pollfd>& fds, const int index, std::vector<Client*>& clients);

// * Status Code Handler ********************************************************************************************************************
const std::string getStatusCodeMessage(const int statusCode);

} // namespace HTTP

# endif