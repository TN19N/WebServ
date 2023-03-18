# pragma once

# include <string>
# include <map>

class IBase {
    public:
        bool                                keepAlive;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        std::string                         buffer;
        bool                                isChunked;

        int     state;
        size_t  contentLength;
        IBase();

        virtual ~IBase();
};