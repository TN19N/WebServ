# pragma once

# include <string>
# include <map>
# include <vector>
# include <climits>
# include <stdlib.h>
# include <algorithm>

class IBase {
	public:
		typedef std::map<std::string, std::string> Headers;
    public:
        bool						keepAlive;
        IBase::Headers				headers;
		std::vector<std::string>	setCookie;
        std::string					body;
        std::string					buffer;
        bool						isChunked;

        int						state;
        size_t					contentLength;
		unsigned long long		maxBodySize;
        IBase();

        virtual ~IBase();
};
