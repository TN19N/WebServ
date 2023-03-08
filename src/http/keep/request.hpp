# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <climits>
#include <fstream>
#include <ios>
#include "context.hpp"

typedef std::map<std::string, std::string> Headers;
struct Request
{
	Request() : content_length(0), is_chunked(false), missing_headers(true), ready_to_response(false) {}
	
	std::string		method;
	std::string		path;
	std::string		query;
	std::string		protocol;
	Headers			headers;
	std::string		body;
	std::string		extension;
	size_t 			content_length;
	int				cgi;
	bool			is_chunked;
	bool			missing_headers;
	bool			ready_to_response;
};

# endif