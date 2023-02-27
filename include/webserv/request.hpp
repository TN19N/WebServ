# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

class Request
{
	private:
		std::string							method;
		std::string							path;
		std::string							query;
		std::string							protocol;
		std::map<std::string, std::string>	headers;
		std::string							body;
		int									contentLength;
		bool			is_chunked;
	public:

};

# endif