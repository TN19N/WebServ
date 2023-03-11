# ifndef HTTP_HPP
# define HTTP_HPP

#ifdef DEBUG
#include <iostream>
#endif

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>
#include <sys/stat.h>
#include <dirent.h>

# include "client.hpp"
# include "context.hpp"
#include "configuration.hpp"

# define END_CHUNKED "0\r\n\r\n"
# define END_HEADERS "\r\n\r\n"

namespace HTTP
{
	// * Connection Handler ***********************************************************************
	void closeConnection(std::vector<pollfd>& fds, const int index, std::vector<Client*>& clients);
	void acceptConnection(const int serverFd, std::vector<Client*>& clients);
	
	// * Status Code Handler ************************************
	const std::string getStatusCodeMessage(const int statusCode);
	
	// * Response Handler ******************
	const bool sendResponse(Client* client);
	
	// * Request Handler ***************
	Request*		requestParser(Client *client);
	void			requestHandler(Client* client, const Context* const configuration);
	const Context*	blockMatchAlgorithm(const Client* client, const Context* const configuration);
	void 			getMethodHandler(Client *client);
	void 			readRequestBodyFromBuffer(Client* client);
	
	// * Tools **********************************************************************************
	Client* getClientWithFd(const int fd, const std::vector<Client*>& clients);
	const std::string getDefaultErrorPage(const int statusCode);
	const std::string getHttpDate();
	int __strcmp_(const char *s1, const char *s2);

} // namespace HTTP

# endif