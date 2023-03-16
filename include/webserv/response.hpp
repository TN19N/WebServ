# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

#include "defines.hpp"

struct Response {
    public :
		int									download_file_fd;
        int                                 statusCode;
        std::map<std::string, std::string>  headers;
        bool                                keepAlive;
        size_t                              contentLength;

        std::string                         buffer;
    public :
        Response(const int statusCode, const bool keepAlive);

        void addHeader(const std::string& key, const std::string& value);
        void addBody(const std::string& body);

        ~Response();
};

# endif