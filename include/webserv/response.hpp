# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>

# define CLOSE_CONNECTION   false
# define KEEP_ALIVE         true

struct Response {
    public :
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