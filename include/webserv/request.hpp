# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

struct Request {
    std::map<std::string, std::string>  headers;
    std::string                         body;
    bool                                keepAlive;
    bool                                isChunked;
    std::string                         method;
    std::string                         path;
    std::string                         query;
    std::string                         protocol;
};

# endif