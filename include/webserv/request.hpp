# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

class REQUEST {
    private:
        std::string                        body;
        std::map<std::string, std::string> headers;
        std::string                        method;
        std::string                        path;
        std::string                        protocol;
}

# endif