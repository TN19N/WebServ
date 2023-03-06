# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

struct Response {
    int                                 statusCode;
    std::map<std::string, std::string>  headers;
    std::string                         body;
    bool                                keepAlive;
};

# endif