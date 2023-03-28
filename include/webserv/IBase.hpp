# pragma once

# include <string>
# include <map>
# include <climits>

#include <algorithm> // for std::min

class IBase {
	public:
		typedef std::map<std::string, std::string> Headers;
    public:
        bool					keepAlive;
        IBase::Headers			headers;
        std::string				body;
        std::string				buffer;
        bool					isChunked;

        int						state;
        size_t					contentLength;
		unsigned long long		maxBodySize;
        IBase();

        virtual ~IBase();
};
