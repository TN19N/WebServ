# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

struct Response {
    public :
		int									file_fd;
        int                                 statusCode;
        std::map<std::string, std::string>  headers;
        bool                                keepAlive;
        size_t                              contentLength;
        std::string                         protocol;

        std::string                         buffer;
    public :
        Response(const int statusCode, const bool keepAlive);

        void addHeader(const std::string& key, const std::string& value);
        void addBody(const std::string& body);

        ~Response();
};

# endif