# ifndef REQUEST_HPP
# define REQUEST_HPP


# include <string>
# include <map>
# include <unistd.h>

# include "defines.hpp"
# include "context.hpp"

struct Request {
    public :
		int									upload_file_fd;
		const Context*						location;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        bool                                keepAlive;
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