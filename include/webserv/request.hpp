# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>


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
	int				content_length;
	bool			is_chunked;
};


# endif
