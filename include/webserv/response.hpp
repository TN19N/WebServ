# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
#include <sys/time.h>

#include "defines.hpp"
# include "IBase.hpp"

struct Response : public IBase {
    public :
		int download_file_fd;
        int statusCode;
    public :
		Response() ;
		Response(bool keepAlive);
        Response(const int statusCode, const bool keepAlive);

        void addHeader(const std::string& key, const std::string& value);
        void addBody(const std::string& body);
		void initializeResponse() ;

        ~Response();
};

# endif