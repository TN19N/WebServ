# ifndef HTTP_HPP
# define HTTP_HPP

#ifdef DEBUG
#include <iostream>
#endif

# include <vector>
# include <string>
# include <poll.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include <fstream>
# include <fcntl.h>
# include <cstring>

# include "defines.hpp"
# include "client.hpp"
# include "context.hpp"
# include "configuration.hpp"

namespace HTTP
{
	// * Connection Handler ***********************************************************************
	void closeConnection(std::vector<pollfd>& fds, const int index, std::vector<Client*>& clients);
	void acceptConnection(const int serverFd, std::vector<Client*>& clients);
	
	// * Status Code Handler ************************************
	const std::string getStatusCodeMessage(const int statusCode);
	
	// * Response Handler ******************
	bool 	responseHandler(Client *client);
	
	// * Request Handler ***************
	IBase* 			baseParser(Client *client);
	Client*			requestHandler(Client* client, const Context* const configuration);
	const Context*	blockMatchAlgorithm(const Client* client, const Context* const configuration);
	const Context*	getMatchLocationContext(const std::vector<Context*> &locations, const std::string &path);
	const Context*  getMatchedServer(const Client* client, const Context* const configuration);
	Client*			getMethodHandler(Client *client);
	Client*			postMethodHandler(Client *client);
	Client*			deleteMethodHandler(Client *client);
	void 			readBodyFromBuffer(Client* client);

	// * CGI ******************************************************************
	Client* cgiExecutor(Client* client, const char *cgiPath, const char *rootDir);
	// * Tools **********************************************************************************
	Client*				getClientWithFd(const int fd, const std::vector<Client*>& clients);
	const std::string	getDefaultErrorPage(const int statusCode);
	const std::string	getHttpDate();
	int					strcmp(const char *s1, const char *s2);
	const char 			*getExtensionFromPath(const char *path);
	int					parseContentLength(const char *str);
	const std::string 	urlDecoding(const std::string& url);
	void 				convertCgiResponseToClientResponse(Client *cgi);
	size_t				getCurrentTimeOnMilliSecond();
	void 				realpath(const char *rel, char *abs);

} // namespace HTTP

# endif
