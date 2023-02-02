# ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <string>
# include <map>

# include "definitions.hpp"
# include "context.hpp"
# include "parser.hpp"

class Webserv {
    private:
        Context*                           mainContext;
        std::map<std::string, std::string> mimeTypes;
    public:
        Webserv(const std::string& configFilePath);

        void run();
};

# endif