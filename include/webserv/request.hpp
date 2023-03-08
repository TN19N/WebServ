# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <map>

# define CLOSE_CONNECTION   false
# define KEEP_ALIVE         true

# define READING_BODY       0
# define REQUEST_READY      1

struct Request {
    public :
        std::map<std::string, std::string>  headers;
        std::string                         body;
        bool                                keepAlive;
        bool                                isChunked;
		bool								readyToResponse;
        std::string                         method;
        std::string                         path;
        std::string                         query;
        std::string                         extension;
        size_t                              contentLength;
        int                                 state;
    public :
        Request();

        ~Request();
};

# endif