# ifndef REQUEST_HPP
# define REQUEST_HPP


# include <string>
# include <map>
# include <unistd.h>

# include "defines.hpp"
# include "context.hpp"

struct Request {
    public :
		const Context*						location;
        bool                                keepAlive;
		int									upload_file_fd;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        bool                                isChunked;
        std::string                         method;
        std::string                         path;
		std::string                         fullPath;
        std::string                         query;
        std::string                         extension;
        size_t                              contentLength;
        int                                 state;
    public :
        Request();

        ~Request();
};

# endif