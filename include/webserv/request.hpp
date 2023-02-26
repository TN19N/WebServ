# ifndef REQUEST_HPP
# define REQUEST_HPP

//#ifdef DEBUG_JCHAKIR
	#include <iostream>
//#endif

# include <string>
# include <map>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#include <fstream>
#include <ios>
#include "context.hpp"

typedef std::map<std::string, std::string> Headers;
class Request
{
public:
	std::string		method;
	std::string		path;
	std::string		query;
	std::string		protocol;
	Headers			headers;
	std::string		body;
	std::string		extension;
	int				content_length;
	bool			is_chunked;
};

# endif
