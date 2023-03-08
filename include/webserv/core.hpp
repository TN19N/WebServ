# ifndef CORE_HPP
# define CORE_HPP

# include <string>
# include <vector>
# include <netdb.h>
# include <poll.h>

# include "webserv/context.hpp"
# include "webserv/client.hpp"

namespace CORE {

// * load Configuration ****************************************************************************************
const Context* loadConfiguration(const std::string& configFilePath, Context* configuration, const size_t depth);

// * Tools ************************************************************************************************************
const std::vector<pollfd> fillFds(const std::vector<int>& serversSocketFd, const std::vector<Client*>& clients);
const std::vector<std::string> split(const std::string& str, const std::string& delimiters);
void listenToSignals();

# ifdef DEBUG
// * DEBUG **********************************************
void display(const Context* context, const size_t level);
void display(const struct addrinfo* addrinfo);
void display(const struct sockaddr* addr);
# endif

} // namespace CORE

# endif